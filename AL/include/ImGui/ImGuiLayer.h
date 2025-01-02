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
#include "Renderer/Scene.h"

namespace ale
{
class AL_API ImGuiLayer : public Layer
{
  public:
	ImGuiLayer();
	~ImGuiLayer() = default;

	void onAttach() override;
	void onDetach() override;
	void onEvent(Event &event) override;
	void onImGuiRender() override;

	void begin();
	static void renderDrawData(Scene* scene, VkCommandBuffer commandBuffer);

	bool show_demo_window = false;

  private:
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  private:
	ImGui_ImplVulkan_InitInfo init_info = {};
	VkCommandPool commandPool;
};
} // namespace ale

#endif