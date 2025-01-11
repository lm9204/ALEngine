#ifndef RENDERER_H
#define RENDERER_H

#include "Core/Base.h"
#include "Renderer/CommandBuffers.h"
#include "Renderer/Common.h"
#include "Renderer/DescriptorSetLayout.h"
#include "Renderer/FrameBuffers.h"
#include "Renderer/Pipeline.h"
#include "Renderer/RenderPass.h"
#include "Renderer/ShaderResourceManager.h"
#include "Renderer/SwapChain.h"
#include "Renderer/SyncObjects.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class Renderer
{
  public:
	static std::unique_ptr<Renderer> createRenderer(GLFWwindow *window);
	~Renderer() = default;
	void cleanup();

	void loadScene(Scene *scene);
	void drawFrame(Scene *scene);
	void recreateSwapChain();

	VkDevice getDevice()
	{
		return device;
	}

	VkRenderPass getRenderPass()
	{
		return imGuiRenderPass;
	}

  private:
	Renderer() = default;

	// Scene *scene;

	GLFWwindow *window;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkDevice device;
	VkCommandPool commandPool;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	std::unique_ptr<SwapChain> m_swapChain;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::unique_ptr<FrameBuffers> m_swapChainFrameBuffers;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	std::unique_ptr<FrameBuffers> m_ImGuiSwapChainFrameBuffers;
	std::vector<VkFramebuffer> imGuiSwapChainFrameBuffers;

	std::unique_ptr<FrameBuffers> m_FinalFrameBuffers;
	std::vector<VkFramebuffer> finalFrameBuffers;
	std::vector<VkImageView> finalImageViews;
	std::vector<VkImage> finalImages;

	std::unique_ptr<RenderPass> m_renderPass;
	VkRenderPass renderPass;

	std::unique_ptr<RenderPass> m_ImGuiRenderPass;
	VkRenderPass imGuiRenderPass;

	std::unique_ptr<RenderPass> m_deferredRenderPass;
	VkRenderPass deferredRenderPass;

	std::unique_ptr<DescriptorSetLayout> m_geometryPassDescriptorSetLayout;
	VkDescriptorSetLayout geometryPassDescriptorSetLayout;

	std::unique_ptr<DescriptorSetLayout> m_lightingPassDescriptorSetLayout;
	VkDescriptorSetLayout lightingPassDescriptorSetLayout;

	std::unique_ptr<Pipeline> m_geometryPassPipeline;
	VkPipelineLayout geometryPassPipelineLayout;
	VkPipeline geometryPassGraphicsPipeline;

	std::unique_ptr<Pipeline> m_lightingPassPipeline;
	VkPipelineLayout lightingPassPipelineLayout;
	VkPipeline lightingPassGraphicsPipeline;

	VkDescriptorPool descriptorPool;

	std::unique_ptr<ShaderResourceManager> m_geometryPassShaderResourceManager;
	std::vector<VkDescriptorSet> geometryPassDescriptorSets;
	std::vector<std::shared_ptr<UniformBuffer>> geometryPassUniformBuffers;

	std::unique_ptr<ShaderResourceManager> m_lightingPassShaderResourceManager;
	std::vector<VkDescriptorSet> lightingPassDescriptorSets;
	std::vector<std::shared_ptr<UniformBuffer>> lightingPassUniformBuffers;

	std::unique_ptr<CommandBuffers> m_commandBuffers;

	std::vector<VkCommandBuffer> commandBuffers;
	std::unique_ptr<SyncObjects> m_syncObjects;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;

	void init(GLFWwindow *window);
	void recordCommandBuffer(Scene *scene, VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void recordDeferredRenderPassCommandBuffer(Scene *scene, VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void recordImGuiCommandBuffer(Scene *scene, VkCommandBuffer commandBuffer, uint32_t imageIndex);
};
} // namespace ale

#endif