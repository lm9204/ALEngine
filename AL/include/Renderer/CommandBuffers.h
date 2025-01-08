#ifndef COMMANDBUFFERS_H
#define COMMANDBUFFERS_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class CommandBuffers
{
  public:
	static std::unique_ptr<CommandBuffers> createCommandBuffers();

	~CommandBuffers() = default;

	void cleanup();

	std::vector<VkCommandBuffer> &getCommandBuffers()
	{
		return commandBuffers;
	}

  private:
	std::vector<VkCommandBuffer> commandBuffers;

	void initCommandBuffers();
};

} // namespace ale

#endif