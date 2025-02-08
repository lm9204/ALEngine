#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"
#include "Renderer/VulkanUtil.h"

namespace ale
{
class RenderPass
{
  public:
	static std::unique_ptr<RenderPass> createRenderPass(VkFormat swapChainImageFormat);
	static std::unique_ptr<RenderPass> createDeferredRenderPass();
	static std::unique_ptr<RenderPass> createImGuiRenderPass(VkFormat swapChainImageFormat);
	static std::unique_ptr<RenderPass> createShadowMapRenderPass();
	static std::unique_ptr<RenderPass> createSphericalMapRenderPass();

	~RenderPass() = default;

	void cleanup();

	VkRenderPass getRenderPass()
	{
		return renderPass;
	}

  private:
	VkRenderPass renderPass;

	void initRenderPass(VkFormat swapChainImageFormat);
	void initDeferredRenderPass();
	void initImGuiRenderPass(VkFormat swapChainImageFormat);
	void initShadowMapRenderPass();
	void initSphericalMapRenderPass();
};
} // namespace ale

#endif