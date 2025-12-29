/**
 * @file VulkanHelpers.hpp
 * @brief Helper utilities for Vulkan operations
 * @details Provides error checking macros and utility functions
 */

#pragma once

#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

/**
 * @namespace VulkanHelpers
 * @brief Utility functions and macros for Vulkan operations
 */
namespace VulkanHelpers
{
    /**
     * @brief Check Vulkan result and throw exception with context on failure
     * @param result The VkResult to check
     * @param operation Description of the operation that was attempted
     * @throws std::runtime_error if result is not VK_SUCCESS
     */
    inline void checkResult(VkResult result, const char *operation)
    {
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error(
                std::string("Vulkan operation failed: ") + operation
                + " (error code: " + std::to_string(result) + ")"
            );
        }
    }

} // namespace VulkanHelpers

/**
 * @def VK_CHECK(result, message)
 * @brief Macro to check VkResult and throw on failure with descriptive message
 * @param result The VkResult to check
 * @param message Description of the operation (string literal)
 * @note This macro makes error checking more concise while preserving context
 *
 * Example usage:
 * @code
 * VK_CHECK(vkCreateDevice(...), "create logical device");
 * @endcode
 */
#define VK_CHECK(result, message) VulkanHelpers::checkResult((result), (message))
