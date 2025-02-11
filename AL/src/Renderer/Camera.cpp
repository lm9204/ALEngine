#include "Renderer/Camera.h"
#include "ALpch.h"

namespace ale
{
Camera::Camera()
{
	updateProjMatrix();
	setViewMatrix(m_cameraPos, m_cameraFront, m_cameraUp);
}

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
	return m_frustum;
}

} // namespace ale