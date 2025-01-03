#include "Renderer/CameraController.h"
#include "Core/Log.h"

#include "Core/App.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include <GLFW/glfw3.h>

namespace ale
{
CameraController::CameraController()
{
	m_AspectRatio = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	m_Camera.setProjMatrix(glm::radians(45.0f), m_AspectRatio, 0.01f, 100.0f);
}

void CameraController::onUpdate(Timestep ts)
{
	if (!m_CameraControl)
		return;

	if (Input::isKeyPressed(Key::W))
	{
		// set camera front
	}
	if (Input::isKeyPressed(Key::S))
	{
		// set camera back
	}

	if (Input::isKeyPressed(Key::A))
	{
		// set camera left
	}
	if (Input::isKeyPressed(Key::D))
	{
		// set camera right
	}
}

void CameraController::onEvent(Event &e)
{
	EventDispatcher dispatcher(e);

	dispatcher.dispatch<MouseButtonPressedEvent>(AL_BIND_EVENT_FN(CameraController::onMousePressed));
	dispatcher.dispatch<MouseButtonReleasedEvent>(AL_BIND_EVENT_FN(CameraController::onMouseReleased));
	dispatcher.dispatch<MouseMovedEvent>(AL_BIND_EVENT_FN(CameraController::onMouseMoved));
	dispatcher.dispatch<WindowResizeEvent>(AL_BIND_EVENT_FN(CameraController::onWindowResized));
}

void CameraController::onResize()
{
}

const Camera &CameraController::getCamera() const
{
	return m_Camera;
}

Camera &CameraController::getCamera()
{
	return m_Camera;
}

void CameraController::setCamera(VkExtent2D swapChainExtent, float fov, float _near, float _far)
{
	m_AspectRatio = swapChainExtent.width / (float)swapChainExtent.height;
	m_Camera.setProjMatrix(fov, m_AspectRatio, _near, _far);
}

bool CameraController::onMousePressed(MouseButtonPressedEvent &e)
{
	if (e.getMouseButton() == Mouse::ButtonRight)
	{
		AL_CORE_INFO("CameraController::onMousePressed");
		m_CameraControl = true;

		double xPos, yPos;
		glfwGetCursorPos(App::get().getWindow().getNativeWindow(), &xPos, &yPos);
		m_prevMousePos = glm::vec2((float)xPos, (float)yPos);
	}
	return false;
}

bool CameraController::onMouseReleased(MouseButtonReleasedEvent &e)
{
	AL_CORE_INFO("CameraController::onMouseReleased");
	m_CameraControl = false;
	return false;
}

bool CameraController::onWindowResized(WindowResizeEvent &e)
{
	return false;
}

bool CameraController::onMouseMoved(MouseMovedEvent &e)
{
	if (m_CameraControl)
	{
		AL_CORE_INFO("CameraController::onMouseMoved");

		glm::vec2 pos = glm::vec2(e.getX(), e.getY());
		glm::vec2 deltaPos = pos - m_prevMousePos;

		// set camera rotation
		m_prevMousePos = pos;
	}
	return false;
}

} // namespace ale