/**
 * @file SwapChain.hpp
 * @brief Swapchain creation and configuration for presentation
 */

#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

/**
 * @namespace SwapChain
 * @brief Creates and configures swapchain for presenting rendered images to screen
 */
namespace SwapChain
{
    // === Window Configuration ===
    
    inline constexpr std::uint32_t DEFAULT_WIDTH = 800;  ///< Initial window width in pixels
    inline constexpr std::uint32_t DEFAULT_HEIGHT = 600; ///< Initial window height in pixels
    /**
     * @struct SwapChainSupportDetails
     * @brief Swapchain capabilities and supported formats/modes
     */
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities; ///< Surface capabilities (min/max images, extents)
        std::vector<VkSurfaceFormatKHR>
            formats; ///< Supported surface formats (pixel format + color space)
        std::vector<VkPresentModeKHR>
            presentModes; ///< Supported presentation modes (immediate, FIFO, mailbox)
    };

    /**
     * @brief Query swapchain support details from physical device
     * @param device Physical device to query
     * @param surface Surface to check support for
     * @return Swapchain support details
     * @details Queries capabilities, formats, and present modes
     */
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice &device, VkSurfaceKHR &surface);

    /**
     * @brief Choose optimal surface format
     * @param availableFormats Available surface formats
     * @return Selected surface format
     * @details Prefers BGRA8 SRGB format with SRGB nonlinear color space
     */
    VkSurfaceFormatKHR
    chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    /**
     * @brief Choose optimal presentation mode
     * @param availablePresentModes Available present modes
     * @return Selected present mode
     * @details Prefers MAILBOX (triple buffering) if available, falls back to FIFO (vsync)
     */
    VkPresentModeKHR
    chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    /**
     * @brief Choose swapchain extent (resolution)
     * @param capabilities Surface capabilities
     * @return Swapchain extent
     * @details Matches window framebuffer size within min/max limits
     */
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    /**
     * @brief Create swapchain with optimal settings
     * @param physicalDevice Physical device
     * @param device Logical device
     * @param surface Window surface
     * @param swapChain Output swapchain handle
     * @param swapChainImages Output vector of swapchain images
     * @param swapChainExtent Output swapchain extent
     * @details Creates swapchain with min image count + 1, selected format/mode/extent
     */
    void createSwapChain(
        VkPhysicalDevice &physicalDevice,
        VkDevice &device,
        VkSurfaceKHR &surface,
        VkSwapchainKHR &swapChain,
        std::vector<VkImage> &swapChainImages,
        VkExtent2D &swapChainExtent
    );
} // namespace SwapChain