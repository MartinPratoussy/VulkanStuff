#pragma once

#include <cstdint>
#include <optional>
#include <vulkan/vulkan_core.h>

namespace Queue
{
    struct FamilyIndices
    {
        std::optional<std::uint32_t> graphicsFamily;
        std::optional<std::uint32_t> presentFamily;

        bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    FamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
} // namespace Queue