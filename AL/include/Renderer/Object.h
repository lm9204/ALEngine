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

	Albedo& getAlbedo() { return m_material->getAlbedo(); }
	NormalMap& getNormalMap() { return m_material->getNormalMap(); }
	Roughness& getRoughness() { return m_material->getRoughness(); }
	Metallic& getMetallic() { return m_material->getMetallic(); }
	AOMap& getAOMap() { return m_material->getAOMap(); }
	HeightMap& getHeightMap() { return m_material->getHeightMap(); }

	void setAlbedo(Albedo albedo) { m_material->setAlbedo(albedo); }
	void setNormalMap(NormalMap normalMap) { m_material->setNormalMap(normalMap); }
	void setRoughness(Roughness roughness) { m_material->setRoughness(roughness); }
	void setMetallic(Metallic metallic) { m_material->setMetallic(metallic); }
	void setAOMap(AOMap aoMap) { m_material->setAOMap(aoMap); }
	void setHeightMap(HeightMap heightMap) { m_material->setHeightMap(heightMap); }


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