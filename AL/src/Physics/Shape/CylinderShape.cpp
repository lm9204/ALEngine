#include "Physics/Shape/CylinderShape.h"
#include "Physics/Contact/Contact.h"

namespace ale
{
CylinderShape::CylinderShape()
{
	m_type = EType::CYLINDER;
}

CylinderShape *CylinderShape::clone() const
{
	void *memory = PhysicsAllocator::m_blockAllocator.allocateBlock(sizeof(CylinderShape));
	CylinderShape *clone = new (static_cast<CylinderShape *>(memory)) CylinderShape();
	*clone = *this;
	return clone;
}

int32_t CylinderShape::getChildCount() const
{
	return 1;
}

void CylinderShape::computeAABB(AABB *aabb, const Transform &xf) const
{
	// update vertices
	std::vector<glm::vec3> vertexVector(m_vertices.begin(), m_vertices.end());
	glm::mat4 transformMatrix = xf.toMatrix();

	glm::vec3 upper(std::numeric_limits<float>::lowest());
	glm::vec3 lower(std::numeric_limits<float>::max());

	// 최적화 여지 있음.
	for (glm::vec3 &vertex : vertexVector)
	{
		glm::vec4 v = transformMatrix * glm::vec4(vertex, 1.0f);
		vertex = glm::vec3(v.x, v.y, v.z);

		upper.x = std::max(upper.x, vertex.x);
		upper.y = std::max(upper.y, vertex.y);
		upper.z = std::max(upper.z, vertex.z);
		lower.x = std::min(lower.x, vertex.x);
		lower.y = std::min(lower.y, vertex.y);
		lower.z = std::min(lower.z, vertex.z);
	}

	aabb->upperBound = upper + glm::vec3(0.1f);
	aabb->lowerBound = lower - glm::vec3(0.1f);
}

// void CylinderShape::computeCylinderFeatures(const std::vector<Vertex> &vertices)
// {
// 	glm::vec3 min(FLT_MAX);
// 	glm::vec3 max(-FLT_MAX);

// 	for (const Vertex &vertex : vertices)
// 	{
// 		max.x = std::max(vertex.position.x, max.x);
// 		max.y = std::max(vertex.position.y, max.y);
// 		max.z = std::max(vertex.position.z, max.z);

// 		min.x = std::min(vertex.position.x, min.x);
// 		min.y = std::min(vertex.position.y, min.y);
// 		min.z = std::min(vertex.position.z, min.z);

// 		m_vertices.insert(vertex.position);
// 	}

// 	m_center = (min + max) / 2.0f;
// 	m_axes[0] = glm::vec3(0.0f, 1.0f, 0.0f);
// 	m_height = max.y - min.y;

// 	m_radius = 0.0f;
// 	glm::vec2 center(m_center.x, m_center.z);
// 	for (const Vertex &vertex : vertices)
// 	{
// 		m_radius = std::max(m_radius, glm::length2(glm::vec2(vertex.position.x, vertex.position.z) - center));
// 	}
// 	m_radius = std::sqrt(m_radius);
// }

void CylinderShape::createCylinderPoints()
{
	int32_t segments = 20;
	float angleStep = 2.0f * glm::pi<float>() / static_cast<float>(segments);
	glm::vec3 xAxis(1.0f, 0.0f, 0.0f);

	glm::vec4 topPoint = glm::vec4(m_center + m_height * 0.5f * m_axes[0] + xAxis * m_radius, 1.0f);
	glm::vec4 bottomPoint = glm::vec4(m_center - m_height * 0.5f * m_axes[0] + xAxis * m_radius, 1.0f);

	glm::quat quat = glm::angleAxis(angleStep / 2.0f, m_axes[0]);
	glm::mat4 mat = glm::toMat4(glm::normalize(quat));
	glm::vec3 dir = mat * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	int32_t base = 0;
	for (int32_t i = base; i < segments; i++)
	{
		float theta = i * angleStep;
		glm::quat orientation = glm::angleAxis(theta, m_axes[0]);
		glm::mat4 rotationMatrix = glm::toMat4(glm::normalize(orientation));
		m_points[i] = rotationMatrix * topPoint;
		m_axes[i + 1] = rotationMatrix * glm::vec4(dir, 1.0f);
	}

	base = segments;
	for (int32_t i = base; i < segments + base; i++)
	{
		float theta = (i - base) * angleStep;
		glm::quat orientation = glm::angleAxis(theta, m_axes[0]);
		glm::mat4 rotationMatrix = glm::toMat4(glm::normalize(orientation));
		m_points[i] = rotationMatrix * bottomPoint;
	}
}

void CylinderShape::setShapeFeatures(const glm::vec3 &center, float radius, float height)
{
	m_center = center;
	m_radius = radius;
	m_height = height;
	createCylinderPoints();
}

// void CylinderShape::setShapeFeatures(const std::vector<Vertex> &vertices)
// {
// 	computeCylinderFeatures(vertices);
// 	createCylinderPoints();
// 	// findAxisByLongestPair(vertices);
// 	// computeCylinderRadius(vertices);
// }

ConvexInfo CylinderShape::getShapeInfo(const Transform &transform) const
{
	glm::mat4 matrix = transform.toMatrix();

	ConvexInfo cylinder;
	cylinder.radius = m_radius;
	cylinder.height = m_height;
	cylinder.center = matrix * glm::vec4(m_center, 1.0f);

	int32_t segments = 20;

	int32_t axesSize = segments + 1;
	cylinder.axesCount = axesSize;
	void *memory = PhysicsAllocator::m_blockAllocator.allocateBlock(sizeof(glm::vec3) * cylinder.axesCount);
	cylinder.axes = static_cast<glm::vec3 *>(memory);

	cylinder.axes[0] = glm::normalize(matrix * glm::vec4(m_axes[0], 0.0f));
	for (int32_t i = 1; i < axesSize; ++i)
	{
		cylinder.axes[i] = matrix * glm::vec4(m_axes[i], 1.0f);
	}

	cylinder.pointsCount = segments * 2;
	memory = PhysicsAllocator::m_blockAllocator.allocateBlock(sizeof(glm::vec3) * cylinder.pointsCount);
	cylinder.points = static_cast<glm::vec3 *>(memory);

	for (int32_t i = 0; i < segments; i++)
	{
		cylinder.points[i] = matrix * glm::vec4(m_points[i], 1.0f);
		cylinder.points[i + segments] = matrix * glm::vec4(m_points[i + segments], 1.0f);
	}

	return cylinder;
}

} // namespace ale