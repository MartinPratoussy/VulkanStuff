/**
 * @file Command.hpp
 * @brief Command buffer management and recording
 */

#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

/**
 * @namespace Command
 * @brief Handles command pool, buffer allocation, and command recording
 */
namespace Command
{
    /**
     * @brief Create command pool for allocating command buffers
     * @param device Logical device
     * @param physicalDevice Physical device for queue family queries
     * @param surface Surface for queue family selection
     * @param commandPool Output command pool handle
     * @details Creates pool with RESET_COMMAND_BUFFER flag for per-frame recording
     */
    void createCommandPool(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkSurfaceKHR &surface,
        VkCommandPool &commandPool
    );

    /**
     * @brief Allocate command buffers from pool
     * @param device Logical device
     * @param commandPool Pool to allocate from
     * @param commandBuffers Output vector of command buffers
     * @param commandBufferCount Number of buffers to allocate
     * @details Allocates primary-level command buffers
     */
    void createCommandBuffers(
        VkDevice &device,
        VkCommandPool &commandPool,
        std::vector<VkCommandBuffer> &commandBuffers,
        std::size_t commandBufferCount
    );

    /**
     * @brief Record rendering commands into command buffer
     * @param commandBuffer Command buffer to record into
     * @param renderPass Render pass to use
     * @param framebuffer Framebuffer containing attachments
     * @param extent Render area extent
     * @param graphicsPipeline Graphics pipeline to bind
     * @param pipelineLayout Pipeline layout for descriptor sets
     * @param vertexBuffer Vertex buffer to bind
     * @param indexBuffer Index buffer to bind
     * @param descriptorSets Descriptor sets to bind
     * @param currentFrame Current frame index for descriptor set selection
     * @details Records begin/end render pass, pipeline binding, draw calls
     */
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

    /**
     * @brief Begin a single-time command buffer for transfers
     * @param device Logical device
     * @param commandPool Command pool to allocate from
     * @return Allocated and begun command buffer
     * @details Used for one-off operations like buffer/image transfers
     */
    VkCommandBuffer beginSingleTimeCommands(VkDevice &device, VkCommandPool &commandPool);

    /**
     * @brief End and submit single-time command buffer
     * @param device Logical device
     * @param commandPool Command pool to free buffer
     * @param commandBuffer Command buffer to end and submit
     * @param graphicsQueue Queue to submit to
     * @details Ends recording, submits, waits for completion, and frees buffer
     */
    void endSingleTimeCommands(
        VkDevice &device,
        VkCommandPool &commandPool,
        VkCommandBuffer &commandBuffer,
        VkQueue &graphicsQueue
    );
} // namespace Command