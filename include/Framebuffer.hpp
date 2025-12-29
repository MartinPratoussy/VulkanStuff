/**
 * @file Framebuffer.hpp
 * @brief Framebuffer creation for render passes
 */

#pragma once

#include <vector>
#include <vulkan/vulkan.h>

/**
 * @namespace Framebuffer
 * @brief Creates framebuffers linking image views to render passes
 */
namespace Framebuffer
{
    /**
     * @brief Create framebuffers for swapchain images
     * @param device Logical device
     * @param renderPass Render pass defining attachment layout
     * @param swapChainImageViews Image views for color attachments
     * @param swapChainExtent Dimensions of framebuffers
     * @param swapChainFramebuffers Output vector of framebuffers
     * @details Creates one framebuffer per swapchain image
     */
    void createFramebuffers(
        VkDevice &device,
        VkRenderPass &renderPass,
        const std::vector<VkImageView> &swapChainImageViews,
        VkExtent2D &swapChainExtent,
        std::vector<VkFramebuffer> &swapChainFramebuffers
    );
} // namespace Framebuffer