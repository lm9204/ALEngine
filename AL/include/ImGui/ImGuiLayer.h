#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Core/Layer.h"
#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "ImGui/ImGuiVulkanGlfw.h"
#include "ImGui/ImGuiVulkanRenderer.h"

#include "Renderer/Common.h"
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

	void beginFrame();
	static void renderDrawData(VkCommandBuffer commandBuffer);
	static void renderTest(VkDescriptorSet descriptorSet, VkCommandBuffer commandBuffer);

	void blockEvents(bool block)
	{
		m_BlockEvents = block;
	}

  private:
	ImGui_ImplVulkan_InitInfo init_info = {};
	bool m_BlockEvents = false; // true default

	// VkDescriptorPool descriptorPool;
};
} // namespace ale

#endif