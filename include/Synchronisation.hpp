#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace Synchronization
{
    void createSyncObjects(
        VkDevice &device,
        std::vector<VkSemaphore> &imageAvailableSemaphores,
        std::vector<VkSemaphore> &renderFinishedSemaphores,
        std::vector<VkFence> &inFlightFences,
        std::size_t maxFramesInFlight
    );
} // namespace Synchronization