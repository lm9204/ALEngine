#include "ImGui/ImGuiLayer.h"
#include "ALpch.h"
#include "Core/App.h"
#include "Events/AppEvent.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

namespace ale
{
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
{
	Renderer &renderer = App::get().getRenderer();
	auto &context = VulkanContext::getContext();
	init_info.Instance = context.getInstance();				// VulkanContext
	init_info.PhysicalDevice = context.getPhysicalDevice(); // renderer
	init_info.Device = context.getDevice();					// renderer
	init_info.QueueFamily = context.getQueueFamily();		// indices.graphicsFamily.value()
	init_info.Queue = context.getGraphicsQueue();			// renderer
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.RenderPass = renderer.getRenderPass(); // vk null handle
	// init_info.Subpass = 1;

	// gui용 descriptor pool 필요
	init_info.DescriptorPool = context.getDescriptorPool(); // renderer
	// >= 2
	init_info.MinImageCount = 2; // 2
	// >= MinImageCount
	// init_info.MSAASamples = VK_SAMPLE_COUNT_8_BIT;
	init_info.ImageCount = init_info.MinImageCount + 1; // min_ImageCount + 1
	// >= VK_SAMPLE_COUNT_1_BIT (0 -> default to VK_SAMPLE_COUNT_1_BIT)
	init_info.Allocator = nullptr; // nullptr
}

void ImGuiLayer::onAttach()
{
	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO();
	// io.IniFilename = nullptr;							  // ini 파일 적용 X
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows

	// Platform_CreateVkSurface 핸들러 등록
	ImGuiPlatformIO &platform_io = ImGui::GetPlatformIO();
	platform_io.Platform_CreateWindow = [](ImGuiViewport *viewport) {
		AL_CORE_INFO("Platform_CreateWindow called for viewport ID: {}", viewport->ID);
		GLFWwindow *window =
			glfwCreateWindow((int)viewport->Size.x, (int)viewport->Size.y, "ImGui Viewport", nullptr, nullptr);
		viewport->PlatformHandle = window;
		viewport->PlatformHandleRaw = window;
	};
	platform_io.Platform_CreateVkSurface = [](ImGuiViewport *vp, ImU64 vk_inst, const void *vk_allocators,
											  ImU64 *out_vk_surface) -> int {
		AL_CORE_INFO("Platform_CreateVkSurface called for viewport ID: {}", vp->ID);
		GLFWwindow *window = (GLFWwindow *)vp->PlatformHandle;
		VkInstance instance = (VkInstance)vk_inst;
		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(instance, window, (const VkAllocationCallbacks *)vk_allocators, &surface) !=
			VK_SUCCESS)
		{
			AL_CORE_ERROR("Failed to create Vulkan surface!");
			return 0;
		}
		*out_vk_surface = (ImU64)surface;
		return 1;
	};

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
	// ones.
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// ImGui Glfw 초기화
	ImGui_ImplGlfw_InitForVulkan(App::get().getWindow().getNativeWindow(), true);

	// ImGui Vulkan 초기화
	ImGui_ImplVulkan_Init(&init_info);

	// ImGui Font 생성
	ImGui_ImplVulkan_CreateFontsTexture();
}

void ImGuiLayer::onDetach()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::onEvent(Event &e)
{
	if (m_BlockEvents)
	{
		ImGuiIO &io = ImGui::GetIO();
		e.m_Handled |= e.isInCategory(EVENT_CATEGORY_MOUSE) & io.WantCaptureMouse;
		e.m_Handled |= e.isInCategory(EVENT_CATEGORY_KEYBOARD) & io.WantCaptureKeyboard;
	}
}

void ImGuiLayer::beginFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::renderDrawData(VkCommandBuffer commandBuffer)
{
	ImGuiIO &io = ImGui::GetIO();

	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiLayer::renderTest(VkDescriptorSet descriptorSet, VkCommandBuffer commandBuffer)
{
	auto &context = VulkanContext::getContext();

	// Viewport 띄운다면 이런식으로
	ImGui::Begin("ALENGINE TEST");

	ImGui::Image(reinterpret_cast<ImTextureID>(descriptorSet), ImVec2{640, 360});

	ImGui::End();

	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

} // namespace ale