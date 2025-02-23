#pragma once

#include "Renderer/Common.h"
#include "Renderer/Texture.h"

#include <filesystem>

namespace ale
{
class ContentBrowserPanel
{
  public:
	ContentBrowserPanel();
	~ContentBrowserPanel();

	void onImGuiRender();

  private:
	ImTextureID createIconTexture(VkImageView imageView, VkSampler sampler);

  private:
	std::filesystem::path m_BaseDirectory;
	std::filesystem::path m_CurrentDirectory;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDevice device;

	VkDescriptorSetLayout iconDescriptorSetLayout;
	VkDescriptorSet directoryDescriptorSet;
	VkDescriptorSet fileDescriptorSet;
	std::shared_ptr<Texture> m_DirectoryIcon;
	std::shared_ptr<Texture> m_FileIcon;
	ImTextureID directoryTextureID;
	ImTextureID fileTextureID;
};

} // namespace ale