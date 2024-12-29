#include "ImGui/ImGuiLayer.h"
#include "ALpch.h"

#include "Events/AppEvent.h"
#include "imgui/imgui.h"

namespace ale
{
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
{
	auto &context = VulkanContext::getContext();
	commandPool = context.getCommandPool();
	init_info.Instance = context.getInstance();								  // VulkanContext
	init_info.PhysicalDevice = context.getPhysicalDevice();					  // renderer
	init_info.Device = context.getDevice();									  // renderer
	init_info.QueueFamily = context.getQueueFamily(init_info.PhysicalDevice); // indices.graphicsFamily.value()
	init_info.Queue = context.getGraphicsQueue();							  // renderer
	init_info.PipelineCache = VK_NULL_HANDLE;								  // vk null handle
	init_info.DescriptorPool = context.getDescriptorPool();					  // renderer
	// >= 2
	init_info.MinImageCount = 2; // 2
	// >= MinImageCount
	init_info.ImageCount = init_info.MinImageCount + 1; // min_ImageCount + 1
	// >= VK_SAMPLE_COUNT_1_BIT (0 -> default to VK_SAMPLE_COUNT_1_BIT)
	init_info.MSAASamples = VK_SAMPLE_COUNT_8_BIT; // msaaSample 값 가져오기
	init_info.Allocator = nullptr;				   // nullptr
}

void ImGuiLayer::onAttach()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO &io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	App &app = App::get();
	Renderer &renderer = app.getRenderer();

	// ImGui Glfw 초기화
	ImGui_ImplGlfw_InitForVulkan(app.getWindow().getWindow(), true);

	AL_CORE_INFO("ImGuiLayer::onAttach");
	// ImGui Vulkan 초기화
	ImGui_ImplVulkan_Init(&init_info, renderer.getRenderPass());

	// ImGui CommandBuffer begin
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	// ImGui Font 생성
	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

	// ImGui CommandBuffer end
	endSingleTimeCommands(commandBuffer);

	// ImGui Font Object 파괴
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImGuiLayer::onDetach()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::onUpdate()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplVulkan_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();
}
void ImGuiLayer::onEvent(Event &event)
{
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

} // namespace ale