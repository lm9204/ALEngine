#pragma once

#include <queue>

#include "Physics/Shape/BoxShape.h"
#include "Physics/Shape/SphereShape.h"

namespace ale
{
class World;
class Fixture;
struct FixtureDef;
struct ContactLink;

enum class EBodyType
{
	STATIC_BODY = 0,
	KINEMATIC_BODY,
	DYNAMIC_BODY
};

enum class EBodyFlag
{
	ISLAND = (1 << 0),
};

struct BodyDef
{
	BodyDef()
	{
		// userData = nullptr;
		// set position
		// position()
		m_position = glm::vec3(0.0f);
		// m_angle = 0.0f;
		m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		m_linearVelocity = glm::vec3(0.0f);
		m_angularVelocity = glm::vec3(0.0f);
		m_linearDamping = 0.0f;
		m_angularDamping = 0.0f;
		m_canSleep = true;
		m_isAwake = true;
		m_useGravity = true;
		m_type = EBodyType::STATIC_BODY;
		m_gravityScale = 15.0f;
		glm::vec3 m_posFreeze = glm::vec3(1.0f);
		glm::vec3 m_rotFreeze = glm::vec3(1.0f);
	}

	EBodyType m_type;
	glm::vec3 m_position;
	glm::quat m_orientation;

	glm::vec3 m_posFreeze;
	glm::vec3 m_rotFreeze;
	// float m_angle;

	glm::vec3 m_linearVelocity;
	glm::vec3 m_angularVelocity;
	float m_linearDamping;
	float m_angularDamping;
	bool m_canSleep;
	bool m_isAwake;
	bool m_useGravity;
	// void *userData;
	float m_gravityScale;
	int32_t m_xfId;
};

class Rigidbody
{
  public:
	Rigidbody(const BodyDef *bd, World *world);
	~Rigidbody();

	void scale(float scale);
	void translate(float distance);
	void addForce(const glm::vec3 &force);
	void addTorque(const glm::vec3 &torque);
	void addGravity();
	void integrate(float duration);
	void updateSweep();
	void registerForce(const glm::vec3 &force);
	void createFixture(Shape *shape);
	void createFixture(const FixtureDef *fd);
	void addForceAtPoint(const glm::vec3 &force, const glm::vec3 &point);
	void addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point);
	void clearAccumulators();
	void synchronizeFixtures();
	void calculateDerivedData();
	void calculateForceAccum();

	bool hasFlag(EBodyFlag flag);
	bool shouldCollide(const Rigidbody *other) const;

	// getter function
	float getInverseMass() const;
	int32_t getTransformId() const;
	int32_t getIslandIndex() const;
	int32_t getBodyId() const;
	EBodyType getType() const;
	ContactLink *getContactLinks();
	glm::vec3 getPointInWorldSpace(const glm::vec3 &point) const;
	const glm::vec3 &getPosition() const;
	const glm::quat &getOrientation() const;
	const Transform &getTransform() const;
	const glm::mat4 &getTransformMatrix() const;
	const glm::vec3 &getLinearVelocity() const;
	const glm::vec3 &getAngularVelocity() const;
	const glm::vec3 &getAcceleration() const;
	const glm::mat3 &getInverseInertiaTensorWorld() const;

	void setFlag(EBodyFlag flag);
	void unsetFlag(EBodyFlag flag);
	void setMassData(float mass, const glm::mat3 &inertiaTensor);
	void setMass(float mass);
	void setPosition(glm::vec3 &position);
	void setIslandIndex(int32_t idx);
	void setOrientation(glm::quat &orientation);
	void setAcceleration(const glm::vec3 &acceleration);
	void setContactLinks(ContactLink *contactLink);
	void setLinearVelocity(glm::vec3 &linearVelocity);
	void setAngularVelocity(glm::vec3 &angularVelocity);
	void setSleep(float duration);
	void setAwake();
	void setRBComponentValue(BodyDef &bdDef);
	bool isAwake();


	Rigidbody *next;
	Rigidbody *prev;

  protected:
	static int32_t BODY_COUNT;
	static const float START_SLEEP_TIME;

	World *m_world;

	Sweep m_sweep;
	Transform m_xf;
	glm::vec3 m_linearVelocity;
	glm::vec3 m_angularVelocity;
	glm::mat3 m_inverseInertiaTensorWorld;
	glm::mat3 m_inverseInertiaTensor;
	glm::mat4 m_transformMatrix;
	glm::vec3 m_forceAccum;
	glm::vec3 m_torqueAccum;
	glm::vec3 m_acceleration;
	glm::vec3 m_lastFrameAcceleration;
	std::queue<glm::vec3> m_forceRegistry;

	int32_t m_fixtureCount;
	Fixture *m_fixtures = nullptr;

	// float motion;
	bool m_isAwake;
	bool m_canSleep;
	bool m_useGravity;
	float m_sleepTime;

	float m_inverseMass;
	float m_linearDamping;
	float m_angularDamping;
	float m_gravityScale;
	int32_t m_xfId;
	int32_t m_flags;
	int32_t m_islandIndex;
	int32_t m_bodyID;
	EBodyType m_type;
	glm::vec3 m_posFreeze;
	glm::vec3 m_rotFreeze;

	ContactLink *m_contactLinks;

  private:
};
} // namespace ale