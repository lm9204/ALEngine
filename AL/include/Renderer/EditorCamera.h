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
	EditorCamera() = default;
	EditorCamera(float fov, float aspect, float _near, float _far);
	virtual ~EditorCamera() = default;
	virtual const Frustum &getFrustum() override;

	void onUpdate(Timestep ts);
	void onEvent(Event &e);
	void onResize();

  private:
	bool onMousePressed(MouseButtonPressedEvent &e);
	bool onMouseReleased(MouseButtonReleasedEvent &e);
	bool onWindowResized(WindowResizeEvent &e);
	bool onMouseMoved(MouseMovedEvent &e);
	void updateView();

  private:
	glm::vec2 m_prevMousePos{0.0f, 0.0f};
	bool m_CameraControl{false};
	const float m_Speed{0.005f};
	const float m_RotSpeed{0.8f};
	float m_CameraPitch{0.0f};
	float m_CameraYaw{0.0f};
};

} // namespace ale