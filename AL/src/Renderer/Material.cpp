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
} // namespace ale