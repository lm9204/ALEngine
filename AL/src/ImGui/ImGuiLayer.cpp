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
	commandPool = context.getCommandPool();
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
	AL_CORE_INFO("ImGuiLayer::onAttach");

	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = nullptr;
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
		AL_CORE_INFO("style ImGuiConfigFlags_ViewportsEnable");
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

void ImGuiLayer::onImGuiRender()
{
	// ImGui::ShowDemoWindow();
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

void ImGuiLayer::begin()
{

	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::renderDrawData(Scene *scene, VkCommandBuffer commandBuffer)
{

	ImGuiIO &io = ImGui::GetIO();

	// {
	// 	App &app = App::get();
	// 	io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

	// 	ImGui::Begin("GUI");
	// 	auto objects = scene->getObjects();
	// 	auto lightObject = scene->getLightObject();

	// 	std::string lightLabelPrefix = lightObject->getName();
	// 	ImGui::Text("Light: %s", lightLabelPrefix.c_str()); // Light object name
	// 	ImGui::Separator();									// Separator for better visibility

	// 	glm::vec3 &lightPosition = lightObject->getPosition();
	// 	if (ImGui::SliderFloat3((lightLabelPrefix + " Position").c_str(), glm::value_ptr(lightPosition), -10.0f, 10.0f))
	// 	{
	// 		lightObject->setPosition(lightPosition);
	// 		scene->updateLightPos(lightPosition);
	// 	}

	// 	glm::vec3 &lightRotation = lightObject->getRotation();
	// 	if (ImGui::SliderFloat3((lightLabelPrefix + " Rotation").c_str(), glm::value_ptr(lightRotation), -180.0f,
	// 							180.0f))
	// 	{
	// 		lightObject->setRotation(lightRotation);
	// 	}

	// 	glm::vec3 &lightScale = lightObject->getScale();
	// 	if (ImGui::SliderFloat3((lightLabelPrefix + " Scale").c_str(), glm::value_ptr(lightScale), 0.1f, 10.0f))
	// 	{
	// 		lightObject->setScale(lightScale);
	// 	}

	// 	for (uint32_t index = 1; index < objects.size(); index++)
	// 	{
	// 		std::string labelPrefix = objects[index]->getName();
	// 		ImGui::Text("Object: %s", labelPrefix.c_str()); // Object name
	// 		ImGui::Separator();								// Separator for better visibility

	// 		glm::vec3 &position = objects[index]->getPosition();
	// 		if (ImGui::SliderFloat3((labelPrefix + " Position").c_str(), glm::value_ptr(position), -10.0f, 10.0f,
	// 								"%.3f", 0.0001f))
	// 		{
	// 			objects[index]->setPosition(position);
	// 		}
	// 		glm::vec3 rotation = objects[index]->getRotation();
	// 		if (ImGui::SliderFloat3((labelPrefix + " Rotation").c_str(), glm::value_ptr(rotation), -180.0f, 180.0f,
	// 								"%.3f", 0.0001f))
	// 		{
	// 			objects[index]->setRotation(rotation);
	// 		}
	// 		glm::vec3 scale = objects[index]->getScale();
	// 		if (ImGui::SliderFloat3((labelPrefix + " Scale").c_str(), glm::value_ptr(scale), 0.1f, 10.0f, "%.3f",
	// 								0.0001f))
	// 		{
	// 			objects[index]->setScale(scale);
	// 		}
	// 		ImGui::Separator(); // Additional separator after each object
	// 	}
	// 	ImGui::End();
	// }

	// VkDescriptorSet ImGui_ImplVulkan_AddTexture(VkSampler sampler, VkImageView image_view, VkImageLayout
	// image_layout)

	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

VkCommandBuffer ImGuiLayer::beginSingleTimeCommands()
{
	AL_CORE_TRACE("ImGuiLayer::beginSingleTimeCommands");

	VkCommandBuffer commandBuffer;

	// 커맨드 버퍼 할당을 위한 구조체
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level =
		VK_COMMAND_BUFFER_LEVEL_PRIMARY; // PRIMARY LEVEL 등록 (해당 커맨드 버퍼가 큐에 단독으로 제출될 수 있음)
	allocInfo.commandPool = commandPool; // 커맨드 풀 지정
	allocInfo.commandBufferCount = 1;	 // 커맨드 버퍼 개수 지정

	// 커맨드 버퍼 생성
	vkAllocateCommandBuffers(init_info.Device, &allocInfo, &commandBuffer);

	// 커맨드 버퍼 기록을 위한 정보 객체
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // 커맨드 버퍼를 1번만 제출

	// GPU에 필요한 작업을 모두 커맨드 버퍼에 기록하기 시작
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}

void ImGuiLayer::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	AL_CORE_TRACE("ImGuiLayer::endSingleTimeCommands");
	// 커맨드 버퍼 기록 중지
	vkEndCommandBuffer(commandBuffer);

	// 복사 커맨드 버퍼 제출 정보 객체 생성
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;			 // 커맨드 버퍼 개수
	submitInfo.pCommandBuffers = &commandBuffer; // 커맨드 버퍼 등록

	vkQueueSubmit(init_info.Queue, 1, &submitInfo, VK_NULL_HANDLE); // 커맨드 버퍼 큐에 제출
	vkQueueWaitIdle(init_info.Queue);								// 그래픽스 큐 작업 종료 대기

	vkFreeCommandBuffers(init_info.Device, commandPool, 1, &commandBuffer); // 커맨드 버퍼 제거
}

void ImGuiLayer::renderTest(VkDescriptorSet descriptorSet, VkCommandBuffer commandBuffer)
{
	auto &context = VulkanContext::getContext();

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