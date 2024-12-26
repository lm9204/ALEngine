#include "ImGui/ImGuiLayer.h"
#include "ALpch.h"

#include "Platform/Vulkan/ImGuiVulkanRenderer.h"
// #include "imgui/imgui.h

namespace ale
{
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
{
}

void ImGuiLayer::onAttach()
{
	// ImGui::CreateContext();
	// ImGui::StyleColorsDark();

	// ImGuiIO &io = ImGui::GetIO();
	// io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	// io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// ImGui_ImplVulkan_Init();
}

void ImGuiLayer::onDetach()
{
}

void ImGuiLayer::onUpdate()
{
}
void ImGuiLayer::onEvent(Event &event)
{
}
} // namespace ale