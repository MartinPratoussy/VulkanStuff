/**
 * @file ValidationLayers.hpp
 * @brief Validation layer configuration for debugging
 */

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

/**
 * @namespace ValidationLayers
 * @brief Manages Vulkan validation layers for error checking and debugging
 */
namespace ValidationLayers
{
    /// List of validation layers to enable (Khronos standard validation)
    extern const std::vector<const char *> validationLayers;

    /// Enable validation layers in debug builds only
    extern const bool enableValidationLayers;

    /**
     * @brief Check if all requested validation layers are supported
     * @return true if all validation layers are available
     * @details Verifies VK_LAYER_KHRONOS_validation is available on the system
     */
    bool checkValidationLayerSupport();
} // namespace ValidationLayers