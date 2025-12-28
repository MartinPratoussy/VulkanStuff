#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace Synchronization
{
    void createSyncObjects(
        VkDevice &device,
        std::vector<VkSemaphore> &imageAvailableSemaphores,      // per frame in flight
        std::vector<VkSemaphore> &renderFinishedSemaphores,     // per swapchain image
        std::vector<VkFence> &inFlightFences,
        std::size_t swapChainImageCount,
        std::size_t maxFramesInFlight
    );
} // namespace Synchronization