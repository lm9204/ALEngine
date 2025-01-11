#include "Renderer/Camera.h"
#include "ALpch.h"

namespace ale
{
void Camera::setProjMatrix(float fov, float aspect, float _near, float _far)
{
	m_Projection = glm::perspective(fov, aspect, _near, _far);
}

void Camera::setPosition(glm::vec3 &pos)
{
	m_Position = pos;
}

} // namespace ale