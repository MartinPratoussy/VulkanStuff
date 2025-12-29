/**
 * @file ImageViews.hpp
 * @brief Image view creation for textures and swapchain images
 */

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

/**
 * @namespace ImageViews
 * @brief Creates image views for accessing images in shaders and framebuffers
 */
namespace ImageViews
{
    /**
     * @brief Create an image view for an image
     * @param device Logical device
     * @param textureImage Image to create view for
     * @param format Image format
     * @return Created image view handle
     * @details Generic image view creation with 2D, single mipmap, single layer
     */
    VkImageView createImageView(VkDevice &device, VkImage &textureImage, VkFormat format);

    /**
     * @brief Create image view specifically for texture
     * @param device Logical device
     * @param textureImage Texture image
     * @param textureImageView Output image view handle
     * @details Convenience wrapper for texture image view creation
     */
    void
    createTextureImageView(VkDevice &device, VkImage &textureImage, VkImageView &textureImageView);

    /**
     * @brief Create image views for all swapchain images
     * @param device Logical device
     * @param swapChainImages Swapchain images
     * @param swapChainImageFormat Format of swapchain images
     * @param swapChainImageViews Output vector of image views
     * @details Creates one image view per swapchain image for framebuffer attachments
     */
    void createImageViews(
        VkDevice &device,
        std::vector<VkImage> &swapChainImages,
        VkFormat swapChainImageFormat,
        std::vector<VkImageView> &swapChainImageViews
    );
} // namespace ImageViews