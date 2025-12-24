#include "Synchronisation.hpp"

void Synchronization::createSyncObjects(VkDevice& device, VkSemaphore& imageAvailableSemaphores, VkSemaphore& renderFinishedSemaphores, VkFence& inFlightFences)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores) != VK_SUCCESS ||
		vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create semaphores!");
	}
}
