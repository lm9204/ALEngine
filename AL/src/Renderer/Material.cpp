#include "Renderer/Material.h"

namespace ale
{
std::shared_ptr<Material> Material::createMaterial(Albedo albedo, NormalMap normalMap, Roughness roughness,
												   Metallic metallic, AOMap aoMap, HeightMap heightMap)
{
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material());
	material->initMaterial(albedo, normalMap, roughness, metallic, aoMap, heightMap);
	return material;
}

void Material::initMaterial(Albedo albedo, NormalMap normalMap, Roughness roughness, Metallic metallic, AOMap aoMap,
							HeightMap heightMap)
{
	m_albedo = albedo;
	m_normalMap = normalMap;
	m_roughness = roughness;
	m_metallic = metallic;
	m_aoMap = aoMap;
	m_heightMap = heightMap;
}

void Material::cleanup()
{
	m_albedo.albedoTexture->cleanup();
	m_normalMap.normalTexture->cleanup();
	m_roughness.roughnessTexture->cleanup();
	m_metallic.metallicTexture->cleanup();
	m_aoMap.aoTexture->cleanup();
	m_heightMap.heightTexture->cleanup();
}
} // namespace ale