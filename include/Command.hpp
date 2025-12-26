#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace Command
{
    void createCommandPool(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkSurfaceKHR &surface,
        VkCommandPool &commandPool
    );

    void createCommandBuffers(
        VkDevice &device,
        VkCommandPool &commandPool,
        std::vector<VkCommandBuffer> &commandBuffers,
        std::size_t commandBufferCount
    );

    void recordCommandBuffer(
        VkCommandBuffer &commandBuffer,
        VkRenderPass &renderPass,
        VkFramebuffer &framebuffer,
        VkExtent2D &extent,
        VkPipeline &graphicsPipeline,
        VkBuffer &vertexBuffer
    );
} // namespace Command