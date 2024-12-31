#ifndef SHADERRESOURCEMANAGER_H
#define SHADERRESOURCEMANAGER_H

#include "Core/Base.h"
#include "Renderer/Buffer.h"
#include "Renderer/Common.h"
#include "Renderer/Scene.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class AL_API ShaderResourceManager
{
  public:
	static std::unique_ptr<ShaderResourceManager> createShaderResourceManager(
		Scene *scene, VkDescriptorSetLayout descriptorSetLayout);
	~ShaderResourceManager() = default;
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

	void initShaderResourceManager(Scene *scene, VkDescriptorSetLayout descriptorSetLayout);
	void createUniformBuffers(Scene *scene);
	void createDescriptorSets(Scene *scene, VkDescriptorSetLayout descriptorSetLayout);
};

} // namespace ale

#endif