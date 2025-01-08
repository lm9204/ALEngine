#ifndef CAMERA_H
#define CAMERA_H

#include "Core/Base.h"
#include "Core/Timestep.h"
#include <glm/glm.hpp>

namespace ale
{

class Camera
{
  public:
	Camera() = default;
	// void move(bool pressW, bool pressS, bool pressD, bool pressA, bool pressE, bool pressQ);
	// void rotate(glm::vec2 &pos);
	void setProjMatrix(float fov, float aspect, float _near, float _far);

	void setPosition(glm::vec3 &pos);
	glm::vec3 &getPosition();

  private:
	glm::mat4 m_ProjMatrix;
	float m_CameraPitch{0.0f};
	float m_CameraYaw{0.0f};
	glm::vec2 prevMousePos{glm::vec2(0.0f)};
	glm::vec3 m_Position{0.0f, 0.0f, 10.0f};
	glm::vec3 m_CameraFront{0.0f, 0.0f, -1.0f};
	glm::vec3 m_CameraUp{0.0f, 1.0f, 0.0f};
	const float m_Speed{0.0025f};
	const float m_RotSpeed{0.1f};
};
} // namespace ale

#endif