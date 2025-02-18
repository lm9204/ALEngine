#include "alpch.h"

#include "Core/App.h"
#include "Project/Project.h"

#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace ale
{

ContentBrowserPanel::ContentBrowserPanel()
	: m_BaseDirectory(Project::getAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
{
	m_DirectoryIcon = Texture::createTexture("Sandbox/Resources/Icons/ContentBrowser/DirectoryIcon.png");
	m_FileIcon = Texture::createTexture("Sandbox/Resources/Icons/ContentBrowser/FileIcon.png");

	auto &context = VulkanContext::getContext();
	descriptorPool = context.getDescriptorPool();
	device = context.getDevice();

	iconDescriptorSetLayout = VulkanUtil::createIconDescriptorSetLayout(device, descriptorPool);
	directoryDescriptorSet =
		VulkanUtil::createIconDescriptorSet(device, descriptorPool, iconDescriptorSetLayout,
											m_DirectoryIcon->getImageView(), m_DirectoryIcon->getSampler());
	fileDescriptorSet = VulkanUtil::createIconDescriptorSet(device, descriptorPool, iconDescriptorSetLayout,
															m_FileIcon->getImageView(), m_FileIcon->getSampler());

	// directoryTextureID = VulkanUtil::createIconTexture(device, descriptorPool, m_DirectoryIcon->getImageView(),
	// 												   m_DirectoryIcon->getSampler());
	// fileTextureID =
	// 	VulkanUtil::createIconTexture(device, descriptorPool, m_FileIcon->getImageView(), m_FileIcon->getSampler());
}

ContentBrowserPanel::~ContentBrowserPanel()
{
	m_DirectoryIcon->cleanup();
	m_FileIcon->cleanup();

	vkDestroyDescriptorSetLayout(device, iconDescriptorSetLayout, nullptr);
}

ImTextureID ContentBrowserPanel::createIconTexture(VkImageView imageView, VkSampler sampler)
{
	VkDescriptorSet descriptorSet;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 0; // 샘플러를 binding 0으로 설정
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	// DescriptorSetLayout 생성
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1; // 샘플러만 포함
	layoutInfo.pBindings = &samplerLayoutBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create sampler-only descriptor set layout!");
	}

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout;

	if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor set!");
	}

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageView = imageView;
	imageInfo.sampler = sampler;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

	return reinterpret_cast<ImTextureID>(descriptorSet);
}

void ContentBrowserPanel::onImGuiRender()
{
	ImGui::Begin("Content Browser");

	if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
	{
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}

	static float padding = 16.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto &path = directoryEntry.path();
		std::string filenameString = path.filename().string();

		ImGui::PushID(filenameString.c_str()); // 각 요소를 구분하기 위해 필요
		ImTextureID icon = directoryEntry.is_directory() ? reinterpret_cast<ImTextureID>(directoryDescriptorSet)
														 : reinterpret_cast<ImTextureID>(fileDescriptorSet);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton("Icon", icon, {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

		if (ImGui::BeginDragDropSource())
		{
			std::filesystem::path relativePath(path);
			const wchar_t *itemPath = relativePath.c_str();
			// KeyString - Value(itemPath) 어디서든 사용 가능
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				m_CurrentDirectory /= path.filename();

			// if script -> turn on code editor
			// if scene -> change scene
		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID(); // PushID()_end
	}

	ImGui::Columns(1);

	// ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	// ImGui::SliderFloat("Padding", &padding, 0, 32);

	// TODO: status bar
	ImGui::End();
}

} // namespace ale