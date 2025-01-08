#ifndef SHADERRESOURCEMANAGER_H
#define SHADERRESOURCEMANAGER_H

#include "Core/Base.h"
#include "Renderer/Buffer.h"
#include "Renderer/Common.h"
#include "Renderer/Scene.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class ShaderResourceManager
{
  public:
	static std::unique_ptr<ShaderResourceManager> createGeometryPassShaderResourceManager(
		Scene *scene, VkDescriptorSetLayout descriptorSetLayout);
	static std::unique_ptr<ShaderResourceManager> createLightingPassShaderResourceManager(
		VkDescriptorSetLayout descriptorSetLayout, VkImageView positionImageView, VkImageView normalImageView,
		VkImageView albedoImageView);
	~ShaderResourceManager()
	{
	}
	void cleanup();

	std::vector<std::shared_ptr<UniformBuffer>> &getUniformBuffers()
	{
		return m_uniformBuffers;
	}
	std::vector<VkDescriptorSet> &getDescriptorSets()
	{
		return descriptorSets;
	}

  private:
	std::vector<std::shared_ptr<UniformBuffer>> m_uniformBuffers;
	std::vector<VkDescriptorSet> descriptorSets;

	void initGeometryPassShaderResourceManager(Scene *scene, VkDescriptorSetLayout descriptorSetLayout);
	void createGeometryPassUniformBuffers(Scene *scene);
	void createGeometryPassDescriptorSets(Scene *scene, VkDescriptorSetLayout descriptorSetLayout);

	void initLightingPassShaderResourceManager(VkDescriptorSetLayout descriptorSetLayout, VkImageView positionImageView,
											   VkImageView normalImageView, VkImageView albedoImageView);
	void createLightingPassUniformBuffers();
	void createLightingPassDescriptorSets(VkDescriptorSetLayout descriptorSetLayout, VkImageView positionImageView,
										  VkImageView normalImageView, VkImageView albedoImageView);
};

} // namespace ale

#endif