#ifndef CAMERA_H
#define CAMERA_H

#include "Core/Base.h"
#include "Core/Timestep.h"
#include "Scene/CullTree.h"
#include <glm/glm.hpp>

namespace ale
{

class Camera
{
  public:
	Camera();
	Camera(float fov, float aspect, float near, float far);
	virtual ~Camera() = default;
	
	void setAspectRatio(float ratio);
	void setFov(float fov);
	void setNear(float _near);
	void setFar(float _far);
	void setProjMatrix(float fov, float aspect, float _near, float _far);
	void setViewMatrix(glm::vec3 &pos, glm::vec3 &dir, glm::vec3 &up);
	void setViewportSize(uint32_t width, uint32_t height);
	void setPosition(glm::vec3 &pos);
	void setRotation(glm::vec3 &rot);
	
	float getFov() const;
	float getNear() const;
	float getFar() const;
	glm::vec3 &getPosition();
	const Frustum &getFrustum();
	const glm::mat4 &getProjection() const;
	const glm::mat4 &getView() const;
	
	void updateProjMatrix();
	void updateViewMatrix();

  protected:
	glm::mat4 m_projection = glm::mat4(1.0f);
	glm::mat4 m_view;

	glm::vec3 m_cameraPos{0.0f, 0.0f, 10.0f};
	glm::vec3 m_cameraFront{0.0f, 0.0f, -1.0f};
	glm::vec3 m_cameraUp{0.0f, 1.0f, 0.0f};

	float m_CameraPitch{0.0f};
	float m_CameraYaw{0.0f};

	Frustum m_frustum;
	float m_fov = glm::radians(45.0f);
	float m_near = 0.001f;
	float m_aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	float m_far = 1000.0f;
};
} // namespace ale

#endif