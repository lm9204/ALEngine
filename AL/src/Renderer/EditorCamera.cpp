#include "Renderer/EditorCamera.h"

#include "Core/App.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace ale
{
EditorCamera::EditorCamera()
{
	// setProjMatrix(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	setViewMatrix(m_CameraPos, m_CameraFront, m_CameraUp);
}

EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
	: m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
{
	// setProjMatrix(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	setViewMatrix(m_CameraPos, m_CameraFront, m_CameraUp);
}

void EditorCamera::onUpdate(Timestep ts)
{
	if (!m_CameraControl)
		return;

	if (Input::isKeyPressed(Key::W))
	{
		m_CameraPos += m_CameraFront * m_Speed * ts.getMiliSeconds();
	}
	if (Input::isKeyPressed(Key::S))
	{
		m_CameraPos -= m_CameraFront * m_Speed * ts.getMiliSeconds();
	}

	auto cameraRight = glm::normalize(glm::cross(m_CameraUp, -m_CameraFront));
	if (Input::isKeyPressed(Key::A))
	{
		m_CameraPos -= cameraRight * m_Speed * ts.getMiliSeconds();
	}
	if (Input::isKeyPressed(Key::D))
	{
		m_CameraPos += cameraRight * m_Speed * ts.getMiliSeconds();
	}
	setPosition(m_CameraPos);
	updateView();
}

void EditorCamera::onEvent(Event &e)
{
	EventDispatcher dispatcher(e);

	dispatcher.dispatch<MouseButtonPressedEvent>(AL_BIND_EVENT_FN(EditorCamera::onMousePressed));
	dispatcher.dispatch<MouseButtonReleasedEvent>(AL_BIND_EVENT_FN(EditorCamera::onMouseReleased));
	dispatcher.dispatch<MouseMovedEvent>(AL_BIND_EVENT_FN(EditorCamera::onMouseMoved));
	dispatcher.dispatch<WindowResizeEvent>(AL_BIND_EVENT_FN(EditorCamera::onWindowResized));
}

void EditorCamera::onResize()
{
}

bool EditorCamera::onMousePressed(MouseButtonPressedEvent &e)
{
	if (e.getMouseButton() == Mouse::ButtonRight)
	{
		m_CameraControl = true;
		m_prevMousePos = Input::getMousePosition();
	}
	return false;
}

bool EditorCamera::onMouseReleased(MouseButtonReleasedEvent &e)
{
	m_CameraControl = false;
	return false;
}

bool EditorCamera::onWindowResized(WindowResizeEvent &e)
{
	return false;
}

bool EditorCamera::onMouseMoved(MouseMovedEvent &e)
{
	if (m_CameraControl)
	{
		glm::vec2 pos = glm::vec2(e.getX(), e.getY());
		glm::vec2 deltaPos = pos - m_prevMousePos;

		// set camera rotation
		m_CameraYaw -= deltaPos.x * m_RotSpeed;
		m_CameraPitch -= deltaPos.y * m_RotSpeed;

		if (m_CameraYaw < 0.0f)
			m_CameraYaw += 360.0f;
		if (m_CameraYaw > 360.0f)
			m_CameraYaw -= 360.0f;

		if (m_CameraPitch > 89.0f)
			m_CameraPitch = 89.0f;
		if (m_CameraPitch < -89.0f)
			m_CameraPitch = -89.0f;
		m_prevMousePos = pos;
	}
	return false;
}

void EditorCamera::updateView()
{
	m_CameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
					glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
					glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	m_CameraPos = m_Position;
	// m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	setViewMatrix(m_CameraPos, m_CameraFront, m_CameraUp);
}

void EditorCamera::updateProj()
{
	// m_AspectRatio = m_ViewportWidth / (float)m_ViewportHeight;
	// setProjMatrix(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
}
} // namespace ale