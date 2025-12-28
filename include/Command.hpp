#pragma once

#include <cstdint>
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
        VkPipelineLayout &pipelineLayout,
        VkBuffer &vertexBuffer,
        VkBuffer &indexBuffer,
        std::vector<VkDescriptorSet> &descriptorSets,
        std::uint32_t currentFrame
    );

    VkCommandBuffer beginSingleTimeCommands(VkDevice &device, VkCommandPool &commandPool);

    void endSingleTimeCommands(
        VkDevice &device,
        VkCommandPool &commandPool,
        VkCommandBuffer &commandBuffer,
        VkQueue &graphicsQueue
    );
} // namespace Command