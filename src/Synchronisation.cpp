#include "Synchronisation.hpp"

#include <ranges>

void Synchronization::createSyncObjects(
    VkDevice &device,
    std::vector<VkSemaphore> &imageAvailableSemaphores,
    std::vector<VkSemaphore> &renderFinishedSemaphores,
    std::vector<VkFence> &inFlightFences,
    std::size_t maxFramesInFlight
)
{
    imageAvailableSemaphores.resize(maxFramesInFlight);
    renderFinishedSemaphores.resize(maxFramesInFlight);
    inFlightFences.resize(maxFramesInFlight);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (auto &&[imageAvailableSemaphore, renderFinishedSemaphore, inFlightFence] :
         std::views::zip(imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences))
    {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore)
                != VK_SUCCESS
            || vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore)
                   != VK_SUCCESS
            || vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphores!");
        }
    }
}
