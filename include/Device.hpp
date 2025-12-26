#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace Device
{
    void pickPhysicalDevice(
        VkInstance &instance, VkPhysicalDevice &physicalDevice, VkSurfaceKHR &surface
    );

    std::uint32_t rateDevice(VkPhysicalDevice device, VkSurfaceKHR surface);

    void createLogicalDevice(
        VkPhysicalDevice physicalDevice,
        VkDevice &device,
        VkQueue &graphicsQueue,
        VkQueue &presentQueue,
        VkSurfaceKHR surface
    );

    bool checkDeviceExtensionSupport(VkPhysicalDevice &device);
} // namespace Device