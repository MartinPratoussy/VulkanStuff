#pragma once

#include <vulkan/vulkan_core.h>

namespace Texture
{
    void createTextureSampler(
        VkDevice &device, VkPhysicalDevice &physicalDevice, VkSampler &textureSampler
    );
} // namespace Texture