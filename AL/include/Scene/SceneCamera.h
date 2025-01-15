#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "Renderer/Camera.h"

namespace ale
{
// camera maybe extended by various types - orthographic, perspective
class SceneCamera : public Camera
{
  public:
	SceneCamera();
	virtual ~SceneCamera() = default;

	void setPerspective(float verticalFOV, float nearClip, float farClip);
	void setViewportSize(uint32_t width, uint32_t height);

	float getPerspectiveVerticalFOV() const
	{
		return m_PerspectiveFOV;
	}
	void setPerspectiveVerticalFOV(float verticalFov)
	{
		m_PerspectiveFOV = verticalFov;
		recalculateProjection();
	}
	float getPerspectiveNearClip() const
	{
		return m_PerspectiveNear;
	}
	void setPerspectiveNearClip(float nearClip)
	{
		m_PerspectiveNear = nearClip;
		recalculateProjection();
	}
	float getPerspectiveFarClip() const
	{
		return m_PerspectiveFar;
	}
	void setPerspectiveFarClip(float farClip)
	{
		m_PerspectiveFar = farClip;
		recalculateProjection();
	}

  private:
	void recalculateProjection();

  private:
	glm::vec3 m_CameraFront{0.0f, 0.1f, -1.0f};
	glm::vec3 m_CameraUp{0.0f, 1.0f, 0.0f};
	glm::vec3 m_CameraPos{0.0f, 0.0f, 10.0f};

	float m_PerspectiveFOV = glm::radians(45.0f);
	float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 100.0f;
	float m_AspectRatio = 0.0f;
};
} // namespace ale

#endif