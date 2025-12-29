/**
 * @file Synchronisation.hpp
 * @brief Synchronization primitives for frame-in-flight rendering
 */

#pragma once

#include <vector>
#include <vulkan/vulkan.h>

/**
 * @namespace Synchronization
 * @brief Creates semaphores and fences for proper GPU-CPU and GPU-GPU synchronization
 */
namespace Synchronization
{
    /**
     * @brief Create all synchronization objects for frame-in-flight rendering
     * @param device Logical device
     * @param imageAvailableSemaphores Output vector of semaphores (per frame in flight)
     * @param renderFinishedSemaphores Output vector of semaphores (per swapchain image)
     * @param inFlightFences Output vector of fences (per frame in flight)
     * @param swapChainImageCount Number of swapchain images
     * @param maxFramesInFlight Maximum concurrent frames
     * @details Creates proper semaphore/fence architecture to prevent validation errors:
     *          - imageAvailableSemaphores: Signaled by vkAcquireNextImageKHR (one per frame)
     *          - renderFinishedSemaphores: Signaled by vkQueueSubmit (one per swapchain image)
     *          - inFlightFences: CPU-GPU synchronization (one per frame)
     * @note CRITICAL: renderFinishedSemaphores indexed by imageIndex prevents reuse before present
     */
    void createSyncObjects(
        VkDevice &device,
        std::vector<VkSemaphore> &imageAvailableSemaphores, // per frame in flight
        std::vector<VkSemaphore> &renderFinishedSemaphores, // per swapchain image
        std::vector<VkFence> &inFlightFences,
        std::size_t swapChainImageCount,
        std::size_t maxFramesInFlight
    );
} // namespace Synchronization