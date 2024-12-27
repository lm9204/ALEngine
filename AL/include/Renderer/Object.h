#ifndef OBJECT_H
#define OBJECT_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Model.h"
#include "Renderer/Texture.h"

namespace ale
{
class AL_API Object
{
  public:
	static std::unique_ptr<Object> createObject(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture,
												glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Object() = default;

	void draw(VkCommandBuffer commandBuffer);

	glm::mat4 getModelMatrix();
	const std::shared_ptr<Texture> &getTexture()
	{
		return m_texture;
	}

  private:
	Object() = default;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Texture> m_texture;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	void initObject(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, glm::vec3 position,
					glm::vec3 rotation, glm::vec3 scale);
};
} // namespace ale

#endif