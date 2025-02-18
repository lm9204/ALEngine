#ifndef VULKANUTIL_H
#define VULKANUTIL_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"
#include <imgui/imgui.h>

namespace ale
{
class VulkanUtil
{
  public:
	static void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
							VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
							VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
									   uint32_t mipLevels);
	static VkFormat findDepthFormat();
	static VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
										VkFormatFeatureFlags features);
	static std::vector<char> readFile(const std::string &filename);
	static void insertImageMemoryBarrier(VkCommandBuffer cmdbuffer, VkImage image, VkAccessFlags srcAccessMask,
										 VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout,
										 VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask,
										 VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange);

	static VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandpool);
	static void endSingleTimeCommands(VkDevice device, VkQueue queue, VkCommandPool commandPool,
									  VkCommandBuffer commandBuffer);

	static VkSampler createSampler();
	static ImTextureID createIconTexture(VkDevice device, VkDescriptorPool descriptorpool, VkImageView imageView,
										 VkSampler sampler);

	static void createCubeMapImage(uint32_t width, uint32_t height, uint32_t mipLevels,
								   VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
								   VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
								   VkDeviceMemory &imageMemory);
	static VkImageView createCubeMapImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
											  uint32_t mipLevels);

	static VkDescriptorSetLayout createIconDescriptorSetLayout(VkDevice device, VkDescriptorPool descriptorPool);
	static VkDescriptorSet createIconDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool,
												   VkDescriptorSetLayout descriptorSetLayout, VkImageView imageView,
												   VkSampler sampler);
};
} // namespace ale

#endif