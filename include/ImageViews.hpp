#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ImageViews
{
    VkImageView createImageView(VkDevice &device, VkImage &textureImage, VkFormat format);

    void
    createTextureImageView(VkDevice &device, VkImage &textureImage, VkImageView &textureImageView);

    void createImageViews(
        VkDevice &device,
        std::vector<VkImage> &swapChainImages,
        VkFormat swapChainImageFormat,
        std::vector<VkImageView> &swapChainImageViews
    );
} // namespace ImageViews