#include "Synchronisation.hpp"

#include <stdexcept>

void Synchronization::createSyncObjects(
    VkDevice &device,
    std::vector<VkSemaphore> &imageAvailableSemaphores,
    std::vector<VkSemaphore> &renderFinishedSemaphores,
    std::vector<VkFence> &inFlightFences,
    std::size_t swapChainImageCount,
    std::size_t maxFramesInFlight
)
{
    // imageAvailableSemaphores: one per frame in flight (cycled through for acquire)
    // renderFinishedSemaphores: one per swapchain image (indexed by imageIndex)
    imageAvailableSemaphores.resize(maxFramesInFlight);
    renderFinishedSemaphores.resize(swapChainImageCount);
    inFlightFences.resize(maxFramesInFlight);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    // Create acquire semaphores (one per frame in flight)
    for (size_t i = 0; i < maxFramesInFlight; i++)
    {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i])
            != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create imageAvailable semaphore!");
        }
    }

    // Create renderFinished semaphores (one per swapchain image)
    for (size_t i = 0; i < swapChainImageCount; i++)
    {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i])
            != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create renderFinished semaphore!");
        }
    }

    // Create fences for each frame in flight
    for (size_t i = 0; i < maxFramesInFlight; i++)
    {
        if (vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create fence!");
        }
    }
}
