#include "Renderer/EditorCamera.h"

#include "Core/App.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace ale
{

EditorCamera::EditorCamera(float fov, float aspect, float _near, float _far) : Camera(fov, aspect, _near, _far) {};

void EditorCamera::onUpdate(Timestep ts)
{
	if (!m_CameraControl)
		return;

	if (Input::isKeyPressed(Key::W))
	{
		m_cameraPos += m_cameraFront * m_Speed * ts.getMiliSeconds();
	}
	if (Input::isKeyPressed(Key::S))
	{
		m_cameraPos -= m_cameraFront * m_Speed * ts.getMiliSeconds();
	}

	auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
	if (Input::isKeyPressed(Key::A))
	{
		m_cameraPos -= cameraRight * m_Speed * ts.getMiliSeconds();
	}
	if (Input::isKeyPressed(Key::D))
	{
		m_cameraPos += cameraRight * m_Speed * ts.getMiliSeconds();
	}

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
	m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
					glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
					glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	// m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	// setViewMatrix(m_cameraPos, m_cameraFront, m_cameraUp);
	updateViewMatrix();
}

const Frustum &EditorCamera::getFrustum()
{
	updateView();
	glm::mat4 toWorldMatrix = glm::inverse(m_view);

	// 0: center, 1: leftUp, 2: leftDown, 3: rightDown, 4: rightUp
	glm::vec3 farPoint[5];
	glm::vec3 nearPoint[5];

	// 0: Y, 1: X
	glm::vec3 farXscale(0.0f);
	glm::vec3 farYscale(0.0f);
	glm::vec3 nearXscale(0.0f);
	glm::vec3 nearYscale(0.0f);

	nearPoint[0] = glm::vec3(0.0f, 0.0f, -1.0f) * m_near;
	farPoint[0] = glm::vec3(0.0f, 0.0f, -1.0f) * m_far;

	// AL_CORE_INFO("near: {}", m_near);
	// AL_CORE_INFO("far: {}", m_far);
	// AL_CORE_INFO("fov: {}", m_fov);
	// AL_CORE_INFO("aspect: {}", m_aspect);
	// AL_CORE_INFO("cameraPos: {}, {}, {}", m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);
	// AL_CORE_INFO("cameraFront: {}, {}, {}", m_cameraFront.x, m_cameraFront.y, m_cameraFront.z);
	// AL_CORE_INFO("cameraUp: {}, {}, {}", m_cameraUp.x, m_cameraUp.y, m_cameraUp.z);

	farYscale.y = m_far * tan(m_fov / 2.0f);
	farXscale.x = m_aspect * farYscale.y;

	nearYscale.y = m_near * tan(m_fov / 2.0f);
	nearXscale.x = m_aspect * nearYscale.y;

	farPoint[1] = toWorldMatrix * glm::vec4((farPoint[0] + farYscale - farXscale), 1.0f);
	farPoint[2] = toWorldMatrix * glm::vec4((farPoint[0] - farYscale - farXscale), 1.0f);
	farPoint[3] = toWorldMatrix * glm::vec4((farPoint[0] - farYscale + farXscale), 1.0f);
	farPoint[4] = toWorldMatrix * glm::vec4((farPoint[0] + farYscale + farXscale), 1.0f);

	// AL_CORE_INFO("\n\n\nfar!!");
	// for (int i = 1; i < 5; i++)
	// {
	// 	AL_CORE_INFO("farPoint[{}]", i);
	// 	AL_CORE_INFO("{}, {}, {}", farPoint[i].x, farPoint[i].y, farPoint[i].z);
	// }

	nearPoint[1] = toWorldMatrix * glm::vec4((nearPoint[0] + nearYscale - nearXscale), 1.0f);
	nearPoint[2] = toWorldMatrix * glm::vec4((nearPoint[0] - nearYscale - nearXscale), 1.0f);
	nearPoint[3] = toWorldMatrix * glm::vec4((nearPoint[0] - nearYscale + nearXscale), 1.0f);
	nearPoint[4] = toWorldMatrix * glm::vec4((nearPoint[0] + nearYscale + nearXscale), 1.0f);

	// AL_CORE_INFO("\n\n\nnear!!");
	// for (int i = 1; i < 5; i++)
	// {
	// 	AL_CORE_INFO("nearPoint[{}]", i);
	// 	AL_CORE_INFO("{}, {}, {}", nearPoint[i].x, nearPoint[i].y, nearPoint[i].z);
	// }

	m_frustum.plane[0] = FrustumPlane(nearPoint[1], nearPoint[2], nearPoint[3]);
	m_frustum.plane[1] = FrustumPlane(farPoint[4], farPoint[3], farPoint[2]);
	m_frustum.plane[2] = FrustumPlane(farPoint[1], farPoint[2], nearPoint[2]);
	m_frustum.plane[3] = FrustumPlane(nearPoint[4], nearPoint[3], farPoint[3]);
	m_frustum.plane[4] = FrustumPlane(farPoint[1], nearPoint[1], nearPoint[4]);
	m_frustum.plane[5] = FrustumPlane(farPoint[3], nearPoint[3], nearPoint[2]);

	// AL_CORE_INFO("\n\n\nplane!!");
	// for (int i = 0; i < 6; i++)
	// {
	// 	AL_CORE_INFO("plane[{}]", i);
	// 	AL_CORE_INFO("normal : {}, {}, {}", m_frustum.plane[i].normal.x, m_frustum.plane[i].normal.y,
	// m_frustum.plane[i].normal.z); 	AL_CORE_INFO("distance: {}", m_frustum.plane[i].distance);
	// }

	return m_frustum;
}

} // namespace ale