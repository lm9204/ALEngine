#include "Physics/Rigidbody.h"
#include "Physics/Fixture.h"
#include "Physics/Shape/Shape.h"
#include "Physics/World.h"

namespace ale
{
static inline void _calculateTransformMatrix(glm::mat4 &transformMatrix, const glm::vec3 &position,
											 const glm::quat &orientation)
{
	glm::mat4 rotationMatrix = glm::toMat4(orientation);

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

	transformMatrix = translationMatrix * rotationMatrix;
}

static inline void _transformInertiaTensor(glm::mat3 &iitWorld, const glm::mat3 &iitBody, const glm::mat4 &rotmat)
{
	glm::mat3 rotationMatrix = glm::mat3(rotmat);

	iitWorld = rotationMatrix * iitBody * glm::transpose(rotationMatrix);
}

int32_t Rigidbody::BODY_COUNT = 0;
const float Rigidbody::START_SLEEP_TIME = 0.3f;

Rigidbody::Rigidbody(const BodyDef *bd, World *world)
{
	this->m_world = world;
	m_type = bd->m_type;
	m_xfId = bd->m_xfId;

	m_xf.position = bd->m_position;
	m_xf.orientation = bd->m_orientation;

	m_linearVelocity = bd->m_linearVelocity;
	m_angularVelocity = bd->m_angularVelocity;

	m_linearDamping = bd->m_linearDamping;
	m_angularDamping = bd->m_angularDamping;
	m_gravityScale = bd->m_gravityScale;

	m_posFreeze = bd->m_posFreeze;
	m_rotFreeze = bd->m_rotFreeze;
	
	m_useGravity = bd->m_useGravity;
	m_canSleep = bd->m_canSleep;
	m_isAwake = bd->m_isAwake;
	m_sleepTime = 0.0f;
	m_acceleration = glm::vec3(0.0f);
	m_flags = 0;
	m_contactLinks = nullptr;
	m_bodyID = BODY_COUNT++;
}

Rigidbody::~Rigidbody()
{
	for (int32_t i = 0; i < m_fixtureCount; ++i)
	{
		m_fixtures[i].destroy();
	}
	PhysicsAllocator::m_blockAllocator.freeBlock(m_fixtures, sizeof(Fixture) * m_fixtureCount);
}

void Rigidbody::synchronizeFixtures()
{
	if (m_type == EBodyType::STATIC_BODY || m_fixtures == nullptr)
	{
		return;
	}

	Transform xf1;
	xf1.position = m_sweep.p;
	xf1.orientation = m_sweep.q;
	BroadPhase *broadPhase = &m_world->m_contactManager.m_broadPhase;

	for (int32_t i = 0; i < m_fixtureCount; ++i)
	{
		m_fixtures[i].synchronize(broadPhase, xf1, m_xf);
	}
}

// Update acceleration by Adding force to Body
void Rigidbody::integrate(float duration)
{
	if (m_type == EBodyType::STATIC_BODY || m_fixtures == nullptr)
	{
		return;
	}

	m_lastFrameAcceleration = m_acceleration;

	// Set acceleration by F = ma
	m_lastFrameAcceleration += (m_forceAccum * m_inverseMass);

	// gravity
	if (m_useGravity)
		addGravity();

	// set angular acceleration
	glm::vec3 angularAcceleration = m_inverseInertiaTensorWorld * m_torqueAccum;

	// set velocity by accerleration
	m_linearVelocity += (m_lastFrameAcceleration * duration) * m_posFreeze;
	m_angularVelocity += (angularAcceleration * duration) * m_rotFreeze;
	
	// impose drag
	m_linearVelocity *= (1.0f - m_linearDamping);
	m_angularVelocity *= (1.0f - m_angularDamping);

	// set sweep (previous Transform)
	m_sweep.p = m_xf.position;
	m_sweep.q = m_xf.orientation;

	// set position
	m_xf.position += (m_linearVelocity * duration);

	// set orientation
	glm::quat angularVelocityQuat = glm::quat(0.0f, m_angularVelocity * duration); // 각속도를 쿼터니언으로 변환
	m_xf.orientation += 0.5f * angularVelocityQuat * m_xf.orientation;			   // 쿼터니언 미분 공식
	m_xf.orientation = glm::normalize(m_xf.orientation);						   // 정규화하여 안정성 유지

	calculateDerivedData();
	clearAccumulators();

	// if can sleep ~
}

void Rigidbody::calculateDerivedData()
{
	glm::quat q = glm::normalize(m_xf.orientation);

	_calculateTransformMatrix(m_transformMatrix, m_xf.position, q);
	_transformInertiaTensor(m_inverseInertiaTensorWorld, m_inverseInertiaTensor, m_transformMatrix);
}

void Rigidbody::addForce(const glm::vec3 &force)
{
	m_forceAccum += force;
}

void Rigidbody::addForceAtPoint(const glm::vec3 &force, const glm::vec3 &point)
{
	glm::vec3 pt = point;
	pt -= m_xf.position;

	m_forceAccum += force;
	m_torqueAccum += glm::cross(pt, force);
}

void Rigidbody::addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point)
{
	glm::vec3 pt = getPointInWorldSpace(point);
	addForceAtPoint(force, pt);
}

void Rigidbody::addTorque(const glm::vec3 &torque)
{
	m_torqueAccum += torque;
}

void Rigidbody::addGravity()
{
	if (m_type == EBodyType::DYNAMIC_BODY)
	{
		m_lastFrameAcceleration += glm::vec3(0.0f, -m_gravityScale, 0.0f);
	}
}

void Rigidbody::calculateForceAccum()
{
	while (!m_forceRegistry.empty())
	{
		addForce(m_forceRegistry.front());
		m_forceRegistry.pop();
	}
}

void Rigidbody::registerForce(const glm::vec3 &force)
{
	setAwake();

	m_forceRegistry.push(force);
}

void Rigidbody::clearAccumulators()
{
	// clear accumulate vector to zero
	m_forceAccum.x = 0;
	m_forceAccum.y = 0;
	m_forceAccum.z = 0;

	m_torqueAccum.x = 0;
	m_torqueAccum.y = 0;
	m_torqueAccum.z = 0;
}

glm::vec3 Rigidbody::getPointInWorldSpace(const glm::vec3 &point) const
{
	glm::vec4 ret = m_transformMatrix * glm::vec4(point, 1.0f);
	return glm::vec3(ret);
}

const Transform &Rigidbody::getTransform() const
{
	return m_xf;
}

const glm::vec3 &Rigidbody::getPosition() const
{
	return m_xf.position;
}

const glm::quat &Rigidbody::getOrientation() const
{
	return m_xf.orientation;
}

const glm::vec3 &Rigidbody::getLinearVelocity() const
{
	return m_linearVelocity;
}

const glm::vec3 &Rigidbody::getAngularVelocity() const
{
	return m_angularVelocity;
}

const glm::mat4 &Rigidbody::getTransformMatrix() const
{
	return m_transformMatrix;
}

const glm::mat3 &Rigidbody::getInverseInertiaTensorWorld() const
{
	return m_inverseInertiaTensorWorld;
}

float Rigidbody::getInverseMass() const
{
	return m_inverseMass;
}

int32_t Rigidbody::getTransformId() const
{
	return m_xfId;
}

int32_t Rigidbody::getIslandIndex() const
{
	return m_islandIndex;
}

ContactLink *Rigidbody::getContactLinks()
{
	return m_contactLinks;
}

EBodyType Rigidbody::getType() const
{
	return m_type;
}

int32_t Rigidbody::getBodyId() const
{
	return m_bodyID;
}

void Rigidbody::setPosition(glm::vec3 &position)
{
	position -= m_xf.position;
	m_xf.position += position * m_posFreeze;
}

void Rigidbody::setOrientation(glm::quat &orientation)
{
	m_xf.orientation = orientation;
}

void Rigidbody::setLinearVelocity(glm::vec3 &linearVelocity)
{
	m_linearVelocity = linearVelocity * m_posFreeze;
}

void Rigidbody::setAngularVelocity(glm::vec3 &angularVelocity)
{
	m_angularVelocity = angularVelocity * m_rotFreeze;
}

void Rigidbody::setMassData(float mass, const glm::mat3 &inertiaTensor)
{
	// 추후 예외처리
	if (mass == 0)
	{
		m_inverseMass = 0.0f;
		m_inverseInertiaTensor = inertiaTensor;
	}
	else
	{
		m_inverseMass = 1 / mass;

		// 역행렬 존재 가능한지 예외처리
		m_inverseInertiaTensor = inertiaTensor;
		m_inverseInertiaTensor = glm::inverse(m_inverseInertiaTensor);
	}
}

void Rigidbody::setMass(float mass)
{
	if (mass == 0)
	{
		m_inverseMass = 0.0f;
	}
	else
	{
		m_inverseMass = 1 / mass;
	}
}

void Rigidbody::setContactLinks(ContactLink *contactLink)
{
	m_contactLinks = contactLink;
}

void Rigidbody::setIslandIndex(int32_t idx)
{
	m_islandIndex = idx;
}

void Rigidbody::setFlag(EBodyFlag flag)
{
	m_flags = m_flags | static_cast<int32_t>(flag);
}

void Rigidbody::unsetFlag(EBodyFlag flag)
{
	m_flags = m_flags & ~static_cast<int32_t>(flag);
}

void Rigidbody::createFixture(Shape *shape)
{
	FixtureDef fd;
	fd.shape = shape;

	createFixture(&fd);
}

void Rigidbody::createFixture(const FixtureDef *fd)
{
	m_fixtureCount = 1;
	void *memory = PhysicsAllocator::m_blockAllocator.allocateBlock(sizeof(Fixture) * m_fixtureCount);
	m_fixtures = static_cast<Fixture *>(memory);

	for (int32_t i = 0; i < m_fixtureCount; i++)
	{
		new (&(m_fixtures[i])) Fixture();
		m_fixtures[i].create(this, fd);
		m_fixtures[i].createProxies(&m_world->m_contactManager.m_broadPhase);
	}
}

bool Rigidbody::hasFlag(EBodyFlag flag)
{
	return (m_flags & static_cast<int32_t>(flag)) == static_cast<int32_t>(flag);
}

void Rigidbody::updateSweep()
{
	m_sweep.p = m_xf.position;
	m_sweep.q = m_xf.orientation;
}

bool Rigidbody::shouldCollide(const Rigidbody *other) const
{
	if (m_type != EBodyType::DYNAMIC_BODY && other->m_type != EBodyType::DYNAMIC_BODY)
	{
		return false;
	}
	return true;
}

void Rigidbody::setSleep(float duration)
{
	if (m_canSleep)
	{
		m_sleepTime += duration;

		if (m_sleepTime > START_SLEEP_TIME)
		{
			m_isAwake = false;
		}
	}
}

void Rigidbody::setAwake()
{
	m_sleepTime = 0.0f;
	m_isAwake = true;
}

void Rigidbody::setRBComponentValue(BodyDef &bdDef)
{
	m_linearDamping = bdDef.m_linearDamping;
	m_angularDamping = bdDef.m_angularDamping;
	m_useGravity = bdDef.m_useGravity;
}

bool Rigidbody::isAwake()
{
	return m_isAwake;
}
} // namespace ale