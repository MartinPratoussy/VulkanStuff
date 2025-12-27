#pragma once

#include <vulkan/vulkan_core.h>
#define STB_IMAGE_IMPLEMENTATION
#include <cstdint>
#include <stb_image.h>

namespace Image
{
    void createTextureImage(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkImage &textureImage,
        VkDeviceMemory &textureImageMemory,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

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

    void transitionImageLayout(
        VkDevice &device,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue,
        VkImage image,
        VkFormat format,
        VkImageLayout oldLayout,
        VkImageLayout newLayout
    );

    void copyBufferToImage(
        VkDevice &device,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue,
        VkBuffer buffer,
        VkImage image,
        uint32_t width,
        uint32_t height
    );
} // namespace Image