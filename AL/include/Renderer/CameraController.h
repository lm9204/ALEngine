#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Core/Base.h"
#include "Core/Timestep.h"
#include "Renderer/Camera.h"
#include "Renderer/Common.h"

#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace ale
{
class AL_API CameraController
{
  public:
	CameraController();

	void onUpdate(Timestep ts);
	void onEvent(Event &e);
	void onResize();
	const Camera &getCamera() const;
	Camera &getCamera();
	void setCamera(VkExtent2D swapChainExtent, float fov, float _near, float _far);

  private:
	bool onMousePressed(MouseButtonPressedEvent &e);
	bool onMouseReleased(MouseButtonReleasedEvent &e);
	bool onWindowResized(WindowResizeEvent &e);
	bool onMouseMoved(MouseMovedEvent &e);

  private:
	Camera m_Camera;
	glm::vec2 m_prevMousePos{0.0f, 0.0f};
	bool m_CameraControl{false};
	float m_AspectRatio;
};

} // namespace ale

#endif