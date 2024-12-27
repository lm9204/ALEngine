#ifndef DESCRIPTORSETLAYOUT_H
#define DESCRIPTORSETLAYOUT_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class AL_API DescriptorSetLayout
{
  public:
	static std::unique_ptr<DescriptorSetLayout> createDescriptorSetLayout();
	~DescriptorSetLayout()
	{
	}
	void cleanup();

	VkDescriptorSetLayout getDescriptorSetLayout()
	{
		return descriptorSetLayout;
	}

  private:
	VkDescriptorSetLayout descriptorSetLayout;

	void initDescriptorSetLayout();
};
} // namespace ale
#endif
