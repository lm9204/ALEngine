#ifndef OBJECT_H
#define OBJECT_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Model.h"
#include "Renderer/Texture.h"
#include "Renderer/Material.h"

namespace ale
{
class AL_API Object
{
public:
	static std::unique_ptr<Object> createObject(std::string name, std::shared_ptr<Model> model, 
	std::shared_ptr<Material> material, Transform transform);

	~Object() {}

	void draw(VkCommandBuffer commandBuffer);

	glm::mat4 getModelMatrix();
	const std::shared_ptr<Texture>& getTexture() { return m_material->getAlbedo().albedoTexture; }

	const std::string& getName() { return m_name; }
	glm::vec3& getPosition() { return m_transform.position; }
	glm::vec3& getRotation() { return m_transform.rotation; }
	glm::vec3& getScale() { return m_transform.scale; }
	void setPosition(glm::vec3 position) { m_transform.position = position; }
	void setRotation(glm::vec3 rotation) { m_transform.rotation = rotation; }
	void setScale(glm::vec3 scale) { m_transform.scale = scale; }


private:
	Object() = default;

	std::shared_ptr<Model> m_model;
	Transform m_transform;
	std::shared_ptr<Material> m_material;
	std::string m_name;

	void initObject(std::string name, std::shared_ptr<Model> model, 
	std::shared_ptr<Material> material, Transform transform);

};
} // namespace ale

#endif