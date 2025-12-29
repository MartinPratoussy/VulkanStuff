/**
 * @file Queue.hpp
 * @brief Queue family discovery and management
 */

#pragma once

#include <cstdint>
#include <optional>
#include <vulkan/vulkan_core.h>

/**
 * @namespace Queue
 * @brief Finds queue families supporting graphics and presentation
 */
namespace Queue
{
    /**
     * @struct FamilyIndices
     * @brief Indices of queue families supporting required operations
     */
    struct FamilyIndices
    {
        std::optional<std::uint32_t> graphicsFamily; ///< Graphics operations queue family
        std::optional<std::uint32_t> presentFamily;  ///< Presentation to surface queue family

        /**
         * @brief Check if all required queue families are found
         * @return true if both graphics and present families are available
         */
        bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    /**
     * @brief Find queue families supporting graphics and presentation
     * @param device Physical device to query
     * @param surface Surface for presentation support check
     * @return Queue family indices
     * @details Searches for families supporting both graphics commands and surface presentation
     */
    FamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
} // namespace Queue