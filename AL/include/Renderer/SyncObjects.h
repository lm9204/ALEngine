#ifndef SYNCOBJECTS_H
#define SYNCOBJECTS_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class AL_API SyncObjects
{
  public:
	static std::unique_ptr<SyncObjects> createSyncObjects();

	~SyncObjects() = default;

	void cleanup();

	std::vector<VkSemaphore> &getImageAvailableSemaphores()
	{
		return imageAvailableSemaphores;
	}
	std::vector<VkSemaphore> &getRenderFinishedSemaphores()
	{
		return renderFinishedSemaphores;
	}
	std::vector<VkFence> &getInFlightFences()
	{
		return inFlightFences;
	}

  private:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	void initSyncObjects();
};
} // namespace ale

#endif