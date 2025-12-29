/**
 * @file Device.hpp
 * @brief Physical and logical device selection and creation
 */

#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>

/**
 * @namespace Device
 * @brief Manages GPU selection and logical device creation
 */
namespace Device
{
    /**
     * @brief Select suitable physical device (GPU)
     * @param instance Vulkan instance
     * @param physicalDevice Output physical device handle
     * @param surface Surface for presentation support check
     * @details Rates all available GPUs and selects the best one
     */
    void pickPhysicalDevice(
        VkInstance &instance, VkPhysicalDevice &physicalDevice, VkSurfaceKHR &surface
    );

    /**
     * @brief Rate a physical device's suitability
     * @param device Physical device to rate
     * @param surface Surface for swapchain support check
     * @return Suitability score (higher is better, 0 = unsuitable)
     * @details Prefers discrete GPUs with swapchain and queue support
     */
    std::uint32_t rateDevice(VkPhysicalDevice device, VkSurfaceKHR surface);

    /**
     * @brief Create logical device with required queues
     * @param physicalDevice Physical device to create from
     * @param device Output logical device handle
     * @param graphicsQueue Output graphics queue handle
     * @param presentQueue Output present queue handle
     * @param surface Surface for queue selection
     * @details Creates device with graphics and present queue families
     */
    void createLogicalDevice(
        VkPhysicalDevice physicalDevice,
        VkDevice &device,
        VkQueue &graphicsQueue,
        VkQueue &presentQueue,
        VkSurfaceKHR surface
    );

    /**
     * @brief Check if device supports required extensions
     * @param device Physical device to check
     * @return true if all required extensions supported
     * @details Verifies VK_KHR_swapchain extension availability
     */
    bool checkDeviceExtensionSupport(VkPhysicalDevice &device);
} // namespace Device