#include "Renderer/Camera.h"
#include "ALpch.h"
#include "Scene/CullTree.h"

namespace ale
{

Camera::Camera(float fov, float aspect, float _near, float _far)
{
	setProjMatrix(fov, aspect, _near, _far);
	setViewMatrix(m_cameraPos, m_cameraFront, m_cameraUp);
}

void Camera::setProjMatrix(float fov, float aspect, float _near, float _far)
{
	m_fov = fov;
	m_aspect = aspect;
	m_near = _near;
	m_far = _far;

	m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

void Camera::setViewMatrix(glm::vec3 &pos, glm::vec3 &dir, glm::vec3 &up)
{
	m_view = glm::lookAt(pos, pos + dir, up);
}

void Camera::setViewportSize(uint32_t width, uint32_t height)
{
	m_aspect = static_cast<float>(width) / static_cast<float>(height);
	updateProjMatrix();
}

void Camera::updateProjMatrix()
{
	m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

void Camera::setPosition(glm::vec3 &pos)
{
	m_cameraPos = pos;
}

void Camera::setFov(float fov)
{
	m_fov = fov;
	updateProjMatrix();
}

void Camera::setNear(float _near)
{
	m_near = _near;
	updateProjMatrix();
}

void Camera::setFar(float _far)
{
	m_far = _far;
	updateProjMatrix();
}

float Camera::getFov() const
{
	return m_fov;
}

float Camera::getNear() const
{
	return m_near;
}

float Camera::getFar() const
{
	return m_far;
}

glm::vec3 &Camera::getPosition()
{
	return m_cameraPos;
}

const glm::mat4 &Camera::getProjection() const
{
	return m_projection;
}

const glm::mat4 &Camera::getView() const
{
	return m_view;
}

const Frustum &Camera::getFrustum()
{
	glm::mat4 toWorldMatrix = glm::inverse(m_view);

	// 0: center, 1: leftUp, 2: leftDown, 3: rightDown, 4: rightUp
	glm::vec3 farPoint[5];
	glm::vec3 nearPoint[5];

	// 0: Y, 1: X
	glm::vec3 farXscale(0.0f);
	glm::vec3 farYscale(0.0f);
	glm::vec3 nearXscale(0.0f);
	glm::vec3 nearYscale(0.0f);

	m_cameraFront = glm::normalize(m_cameraFront);
	nearPoint[0] = m_cameraFront * m_near;
	farPoint[0] = m_cameraFront * m_far;

	farYscale.y = m_far * tan(m_fov / 2.0f);
	farXscale.x = m_aspect * farYscale.y;

	nearYscale.y = m_near * tan(m_fov / 2.0f);
	nearXscale.x = m_aspect * nearYscale.y;

	farPoint[1] = toWorldMatrix * glm::vec4((farPoint[0] + farYscale - farXscale), 1.0f);
	farPoint[2] = toWorldMatrix * glm::vec4((farPoint[0] - farYscale - farXscale), 1.0f);
	farPoint[3] = toWorldMatrix * glm::vec4((farPoint[0] - farYscale + farXscale), 1.0f);
	farPoint[4] = toWorldMatrix * glm::vec4((farPoint[0] + farYscale + farXscale), 1.0f);

	nearPoint[1] = toWorldMatrix * glm::vec4((nearPoint[0] + nearYscale - nearXscale), 1.0f);
	nearPoint[2] = toWorldMatrix * glm::vec4((nearPoint[0] - nearYscale - nearXscale), 1.0f);
	nearPoint[3] = toWorldMatrix * glm::vec4((nearPoint[0] - nearYscale + nearXscale), 1.0f);
	nearPoint[4] = toWorldMatrix * glm::vec4((nearPoint[0] + nearYscale + nearXscale), 1.0f);

	m_frustum.plane[0] = FrustumPlane(nearPoint[1], nearPoint[2], nearPoint[3]);
	m_frustum.plane[1] = FrustumPlane(farPoint[4], farPoint[3], farPoint[2]);
	m_frustum.plane[2] = FrustumPlane(farPoint[1], farPoint[2], nearPoint[2]);
	m_frustum.plane[3] = FrustumPlane(nearPoint[4], nearPoint[3], farPoint[3]);
	m_frustum.plane[4] = FrustumPlane(farPoint[1], nearPoint[1], nearPoint[4]);
	m_frustum.plane[5] = FrustumPlane(farPoint[3], nearPoint[3], nearPoint[2]);

	return m_frustum;
}

} // namespace ale