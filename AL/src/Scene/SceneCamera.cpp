#include "Scene/SceneCamera.h"
#include "Alpch.h"

namespace ale
{
SceneCamera::SceneCamera()
{
	recalculateProjection();
}

void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip)
{
	m_PerspectiveFOV = verticalFOV;
	m_PerspectiveNear = nearClip;
	m_PerspectiveFar = farClip;
	recalculateProjection();
}

void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
{
	m_AspectRatio = (float)width / (float)height;
	recalculateProjection();
}

void SceneCamera::recalculateProjection()
{
	m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
}
} // namespace ale