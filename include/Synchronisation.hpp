#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

namespace Synchronization
{
	void createSyncObjects(VkDevice& device, VkSemaphore& imageAvailableSemaphores, VkSemaphore& renderFinishedSemaphores, VkFence& inFlightFences);
} // namespace Synchronization