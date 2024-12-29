#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Core/App.h"
#include "Core/Layer.h"
#include "Platform/Vulkan/ImGuiVulkanGlfw.h"
#include "Platform/Vulkan/ImGuiVulkanRenderer.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class AL_API ImGuiLayer : public Layer
{
  public:
	ImGuiLayer();
	~ImGuiLayer() = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;
	void onEvent(Event &event) override;

	bool show_demo_window = false;

  private:
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	ImGui_ImplVulkan_InitInfo init_info = {};
	VkCommandPool commandPool;
};
} // namespace ale

#endif