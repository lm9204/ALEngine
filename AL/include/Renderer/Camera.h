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
	virtual ~Camera() = default;

	void setProjMatrix(float fov, float aspect, float _near, float _far);
	void setViewMatrix(glm::vec3 &pos, glm::vec3 &dir, glm::vec3 &up);
	void setPosition(glm::vec3 &pos);

	glm::vec3 &getPosition()
	{
		return m_Position;
	}

	const glm::mat4 &getProjection() const
	{
		return m_Projection;
	}

	const glm::mat4 &getView() const
	{
		return m_View;
	}

  protected:
	glm::mat4 m_Projection = glm::mat4(1.0f);
	glm::mat4 m_View;
	glm::vec3 m_Position{0.0f, 0.0f, 10.0f};
};
} // namespace ale

#endif