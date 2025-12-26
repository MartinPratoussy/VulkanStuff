#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ImageViews
{
    void createImageViews(
        VkDevice &device,
        const std::vector<VkImage> &swapChainImages,
        VkFormat swapChainImageFormat,
        std::vector<VkImageView> &swapChainImageViews
    );
}