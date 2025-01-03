#include "Renderer/Camera.h"
#include "ALpch.h"

namespace ale
{
void Camera::setProjMatrix(float fov, float aspect, float _near, float _far)
{
	m_ProjMatrix = glm::perspective(fov, aspect, _near, _far);
}

glm::mat4 Camera::getViewMatrix()
{
	m_CameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
					glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
					glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	return glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
}

glm::mat4 Camera::getProjMatrix()
{
	return m_ProjMatrix;
}
} // namespace ale