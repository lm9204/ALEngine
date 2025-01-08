#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Core/Layer.h"
#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "ImGui/ImGuiVulkanGlfw.h"
#include "ImGui/ImGuiVulkanRenderer.h"

#include "Renderer/Common.h"
#include "Renderer/Scene.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class ImGuiLayer : public Layer
{
  public:
	ImGuiLayer();
	~ImGuiLayer() = default;

	void onAttach() override;
	void onDetach() override;
	void onEvent(Event &event) override;
	void onImGuiRender() override;

	void begin();
	static void renderDrawData(Scene *scene, VkCommandBuffer commandBuffer);
	static void renderTest(VkDescriptorSet descriptorSet, VkCommandBuffer commandBuffer);

	void blockEvents(bool block)
	{
		m_BlockEvents = block;
	}

	bool show_demo_window = false;

  private:
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  private:
	ImGui_ImplVulkan_InitInfo init_info = {};
	VkCommandPool commandPool;
	bool m_BlockEvents = false; // true default

	// VkDescriptorPool descriptorPool;
};
} // namespace ale

#endif