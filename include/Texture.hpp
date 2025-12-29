/**
 * @file Texture.hpp
 * @brief Texture sampler creation for filtering and addressing
 */

#pragma once

#include <vulkan/vulkan_core.h>

/**
 * @namespace Texture
 * @brief Creates texture samplers for shader texture access
 */
namespace Texture
{
    /**
     * @brief Create texture sampler with filtering and addressing modes
     * @param device Logical device
     * @param physicalDevice Physical device for querying max anisotropy
     * @param textureSampler Output sampler handle
     * @details Creates sampler with:
     *          - Linear filtering (mag/min)
     *          - Repeat addressing mode
     *          - Anisotropic filtering enabled (max device support)
     *          - Unnormalized coordinates disabled (0-1 range)
     */
    void createTextureSampler(
        VkDevice &device, VkPhysicalDevice &physicalDevice, VkSampler &textureSampler
    );
} // namespace Texture