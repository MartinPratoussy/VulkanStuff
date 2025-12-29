#include "Queue.hpp"

#include <optional>
#include <ranges>
#include <vector>

namespace Queue
{
    FamilyIndices findQueueFamilies(const VkPhysicalDevice device, const VkSurfaceKHR surface)
    {
        FamilyIndices indices;

        std::uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (const auto &[i, queueFamily] : std::views::enumerate(queueFamilies))
        {
            if (indices.isComplete())
                break;

            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = static_cast<std::uint32_t>(i);
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport)
            {
                indices.presentFamily = static_cast<std::uint32_t>(i);
            }
        }

        return indices;
    }
} // namespace Queue