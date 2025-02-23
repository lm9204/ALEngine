#include "Renderer/Camera.h"
#include "ALpch.h"

namespace ale
{
Camera::Camera()
{
	updateProjMatrix();
	setViewMatrix(m_cameraPos, m_cameraFront, m_cameraUp);
}

Camera::Camera(float fov, float aspect, float _near, float _far)
{
	setProjMatrix(fov, aspect, _near, _far);
	setViewMatrix(m_cameraPos, m_cameraFront, m_cameraUp);
}

void Camera::setProjMatrix(float fov, float aspect, float _near, float _far)
{
	m_fov = fov;
	m_aspect = aspect;
	m_near = _near;
	m_far = _far;

	m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

void Camera::setViewMatrix(glm::vec3 &pos, glm::vec3 &dir, glm::vec3 &up)
{
	m_view = glm::lookAt(pos, pos + dir, up);
}

void Camera::setViewportSize(uint32_t width, uint32_t height)
{
	m_aspect = static_cast<float>(width) / static_cast<float>(height);
	updateProjMatrix();
}

void Camera::updateProjMatrix()
{
	m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

void Camera::updateViewMatrix()
{
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::setPosition(glm::vec3 &pos)
{
	m_cameraPos = pos;
}

void Camera::setRotation(glm::vec3 &rot)
{
	auto &rotation = glm::degrees(rot);
	m_CameraYaw = rotation.x;
	m_CameraPitch = rotation.y;

	if (m_CameraYaw < 0.0f)
		m_CameraYaw += 360.0f;
	if (m_CameraYaw > 360.0f)
		m_CameraYaw -= 360.0f;

	if (m_CameraPitch > 89.0f)
		m_CameraPitch = 89.0f;
	if (m_CameraPitch < -89.0f)
		m_CameraPitch = -89.0f;

	m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
					glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
					glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
}

void Camera::setAspectRatio(float ratio)
{
	m_aspect = ratio;
	updateProjMatrix();
}

void Camera::setFov(float fov)
{
	m_fov = fov;
	updateProjMatrix();
}

void Camera::setNear(float _near)
{
	m_near = _near;
	updateProjMatrix();
}

void Camera::setFar(float _far)
{
	m_far = _far;
	updateProjMatrix();
}

float Camera::getFov() const
{
	return m_fov;
}

float Camera::getNear() const
{
	return m_near;
}

float Camera::getFar() const
{
	return m_far;
}

glm::vec3 &Camera::getPosition()
{
	return m_cameraPos;
}

const glm::mat4 &Camera::getProjection() const
{
	return m_projection;
}

const glm::mat4 &Camera::getView() const
{
	return m_view;
}

const Frustum &Camera::getFrustum()
{
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

	farYscale.y = m_far * tan(m_fov * 0.5f);
	farXscale.x = m_aspect * farYscale.y;

	nearYscale.y = m_near * tan(m_fov * 0.5f);
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
	m_frustum.plane[4] = FrustumPlane(farPoint[4], farPoint[1], nearPoint[1]);
	m_frustum.plane[5] = FrustumPlane(farPoint[2], farPoint[3], nearPoint[3]);

	// AL_CORE_INFO("\n\n\nplane!!");
	// for (int i = 0; i < 6; i++)
	// {
	// 	AL_CORE_INFO("plane[{}]", i);
	// 	AL_CORE_INFO("normal : {}, {}, {}", m_frustum.plane[i].normal.x, m_frustum.plane[i].normal.y,
	// 				 m_frustum.plane[i].normal.z);
	// 	AL_CORE_INFO("distance: {}", m_frustum.plane[i].distance);
	// }

	return m_frustum;
}

} // namespace ale