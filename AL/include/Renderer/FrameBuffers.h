#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/SwapChain.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class AL_API FrameBuffers
{
  public:
	static std::unique_ptr<FrameBuffers> createSwapChainFrameBuffers(SwapChain *swapChain, VkRenderPass renderPass);

	~FrameBuffers() = default;

	void cleanup();

	void initSwapChainFrameBuffers(SwapChain *swapChain, VkRenderPass renderPass);

	std::vector<VkFramebuffer> &getFramebuffers()
	{
		return framebuffers;
	}

  private:
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
	std::vector<VkFramebuffer> framebuffers;

	VkImage resolveImage;
	VkDeviceMemory resolveImageMemory;
	VkImageView resolveImageView;
};
} // namespace ale

#endif