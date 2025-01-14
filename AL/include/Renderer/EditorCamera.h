#pragma once

#include "Core/Base.h"
#include "Core/Timestep.h"
#include "Renderer/Camera.h"
#include "Renderer/Common.h"

#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace ale
{
class EditorCamera : public Camera
{
  public:
	EditorCamera();
	EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

	void onUpdate(Timestep ts);
	void onEvent(Event &e);
	void onResize();
	void setViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width, m_ViewportHeight = height;
		updateProj();
	}

  private:
	bool onMousePressed(MouseButtonPressedEvent &e);
	bool onMouseReleased(MouseButtonReleasedEvent &e);
	bool onWindowResized(WindowResizeEvent &e);
	bool onMouseMoved(MouseMovedEvent &e);
	void updateView();
	void updateProj();

  private:
	float m_Fov = glm::radians(45.0f);
	float m_AspectRatio = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	float m_NearClip = 0.01F;
	float m_FarClip = 100.0F;

	glm::vec3 m_CameraPos{0.0f, 0.0f, 10.0f};
	glm::vec2 m_prevMousePos{0.0f, 0.0f};
	bool m_CameraControl{false};
	glm::vec3 m_CameraFront{0.0f, 0.0f, -1.0f};
	glm::vec3 m_CameraUp{0.0f, 1.0f, 0.0f};
	const float m_Speed{0.05f};
	const float m_RotSpeed{0.8f};
	float m_CameraPitch{0.0f};
	float m_CameraYaw{0.0f};
	uint32_t m_ViewportWidth = WINDOW_WIDTH, m_ViewportHeight = WINDOW_HEIGHT;
};

} // namespace ale