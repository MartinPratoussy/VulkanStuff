/**
 * @file Image.hpp
 * @brief Image creation and layout transitions for textures
 */

#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>

/**
 * @namespace Image
 * @brief Handles image creation, layout transitions, and buffer-to-image copies
 */
namespace Image
{
    /**
     * @brief Load and create texture image from file
     * @param device Logical device
     * @param physicalDevice Physical device for memory queries
     * @param textureImage Output texture image handle
     * @param textureImageMemory Output image memory handle
     * @param commandPool Command pool for transfer operations
     * @param graphicsQueue Queue for executing transfers
     * @details Loads JPG from disk, uses staging buffer, transitions layouts for optimal access
     */
    void createTextureImage(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkImage &textureImage,
        VkDeviceMemory &textureImageMemory,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

    /**
     * @brief Create a Vulkan image with specified properties
     * @param device Logical device
     * @param physicalDevice Physical device for memory allocation
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param format Pixel format (e.g., R8G8B8A8_SRGB)
     * @param tiling Memory tiling mode (linear vs optimal)
     * @param usage Image usage flags (transfer dst, sampled, etc.)
     * @param properties Memory properties (device local, host visible, etc.)
     * @param image Output image handle
     * @param imageMemory Output memory handle
     * @details Generic image creation used for textures and framebuffers
     */
    void createImage(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        std::uint32_t width,
        std::uint32_t height,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkImage &image,
        VkDeviceMemory &imageMemory
    );

    /**
     * @brief Transition image layout using pipeline barriers
     * @param device Logical device
     * @param commandPool Command pool for transition command
     * @param graphicsQueue Queue to submit transition
     * @param image Image to transition
     * @param format Image format
     * @param oldLayout Current layout
     * @param newLayout Desired layout
     * @details Synchronizes access between layout transitions (undefined -> transfer dst -> shader read)
     */
    void transitionImageLayout(
        VkDevice &device,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue,
        VkImage image,
        VkFormat format,
        VkImageLayout oldLayout,
        VkImageLayout newLayout
    );

    /**
     * @brief Copy data from buffer to image
     * @param device Logical device
     * @param commandPool Command pool for copy command
     * @param graphicsQueue Queue to submit copy
     * @param buffer Source buffer containing pixel data
     * @param image Destination image
     * @param width Image width
     * @param height Image height
     * @details Used after staging buffer upload to transfer texture data to GPU image
     */
    void copyBufferToImage(
        VkDevice &device,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue,
        VkBuffer buffer,
        VkImage image,
        uint32_t width,
        uint32_t height
    );

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
} // namespace Image