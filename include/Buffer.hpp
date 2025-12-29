/**
 * @file Buffer.hpp
 * @brief Buffer management for vertex, index, uniform, and descriptor resources
 */

#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include <array>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

/**
 * @namespace Buffer
 * @brief Manages all buffer-related operations including vertex, index, uniform, and descriptor resources
 */
namespace Buffer
{
    /**
     * @struct Vertex
     * @brief Vertex data structure with position, color, and texture coordinates
     */
    struct Vertex
    {
        glm::vec2 pos;      ///< 2D position in normalized device coordinates
        glm::vec3 color;    ///< RGB color (also used for testing/debugging)
        glm::vec2 texCoord; ///< Texture coordinates (0-1 range)

        /**
         * @struct UniformBufferObject
         * @brief Transformation matrices passed to vertex shader
         * @details Matrices must be 16-byte aligned for std140 layout in GLSL
         */
        struct UniformBufferObject
        {
            alignas(16) glm::mat4 model; ///< Model matrix (object to world transform)
            alignas(16) glm::mat4 view;  ///< View matrix (world to camera transform)
            alignas(16) glm::mat4 proj;  ///< Projection matrix (camera to clip space)
        };

        /**
         * @brief Get vertex input binding description
         * @return Binding description for vertex buffer
         * @details Specifies stride and input rate for vertex data
         */
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        /**
         * @brief Get vertex attribute descriptions
         * @return Array of 3 attribute descriptions (position, color, texCoord)
         * @details Specifies format, location, and offset for each vertex attribute
         */
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };

    /// Quad vertices (two triangles) with position, color, and texture coordinates
    const std::vector<Vertex> vertices =
        {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, ///< Bottom-left (red)
         {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  ///< Bottom-right (green)
         {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},   ///< Top-right (blue)
         {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}}; ///< Top-left (white)

    /// Index buffer for two triangles forming a quad
    const std::vector<std::uint16_t> indices = {0, 1, 2, 2, 3, 0};

    /**
     * @brief Create vertex buffer on GPU with staging buffer transfer
     * @param device Logical device
     * @param physicalDevice Physical device for memory properties
     * @param vertexBuffer Output vertex buffer handle
     * @param vertexBufferMemory Output memory handle
     * @param commandPool Command pool for transfer operations
     * @param graphicsQueue Queue for executing transfer commands
     * @details Uses staging buffer in host-visible memory, then transfers to device-local memory
     */
    void createVertexBuffer(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkBuffer &vertexBuffer,
        VkDeviceMemory &vertexBufferMemory,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

    /**
     * @brief Create index buffer on GPU with staging buffer transfer
     * @param device Logical device
     * @param physicalDevice Physical device for memory properties
     * @param indexBuffer Output index buffer handle
     * @param indexBufferMemory Output memory handle
     * @param commandPool Command pool for transfer operations
     * @param graphicsQueue Queue for executing transfer commands
     * @details Transfers index data from CPU to GPU using staging buffer
     */
    void createIndexBuffer(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkBuffer &indexBuffer,
        VkDeviceMemory &indexBufferMemory,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

    /**
     * @brief Create a Vulkan buffer with specified usage and memory properties
     * @param device Logical device
     * @param physicalDevice Physical device for querying memory types
     * @param size Size of buffer in bytes
     * @param usage Buffer usage flags (vertex, index, uniform, transfer, etc.)
     * @param properties Memory property flags (device local, host visible, etc.)
     * @param vertexBuffer Output buffer handle
     * @param vertexBufferMemory Output memory handle
     * @details Generic buffer creation utility used by all buffer types
     */
    void createBuffer(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer &vertexBuffer,
        VkDeviceMemory &vertexBufferMemory
    );

    /**
     * @brief Copy data from one buffer to another using command buffer
     * @param device Logical device
     * @param srcBuffer Source buffer
     * @param dstBuffer Destination buffer
     * @param size Number of bytes to copy
     * @param commandPool Command pool for allocation
     * @param graphicsQueue Queue to submit copy command
     * @details Uses single-time command buffer for transfer operation
     */
    void copyBuffer(
        VkDevice &device,
        VkBuffer srcBuffer,
        VkBuffer dstBuffer,
        VkDeviceSize size,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

    /**
     * @brief Find suitable memory type index
     * @param physicalDevice Physical device to query
     * @param typeFilter Bit field of suitable memory types
     * @param properties Required memory properties
     * @return Index of suitable memory type
     * @throws std::runtime_error if no suitable memory type found
     */
    std::uint32_t findMemoryType(
        VkPhysicalDevice &physicalDevice, std::uint32_t typeFilter, VkMemoryPropertyFlags properties
    );

    /**
     * @brief Create descriptor set layout
     * @param device Logical device
     * @param descriptorLayout Output descriptor set layout
     * @details Defines bindings for uniform buffer (MVP matrices) and combined image sampler (texture)
     */
    void createDescriptorSetLayout(VkDevice &device, VkDescriptorSetLayout &descriptorLayout);

    /**
     * @brief Create uniform buffers for each frame in flight
     * @param device Logical device
     * @param physicalDevice Physical device for memory queries
     * @param uniformBuffers Output vector of uniform buffers
     * @param uniformBuffersMemory Output vector of memory handles
     * @param uniformBuffersMapped Output vector of mapped pointers for updates
     * @details Creates persistently mapped host-visible buffers for efficient per-frame updates
     */
    void createUniformBuffers(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        std::vector<VkBuffer> &uniformBuffers,
        std::vector<VkDeviceMemory> &uniformBuffersMemory,
        std::vector<void *> &uniformBuffersMapped
    );

    /**
     * @brief Create descriptor pool for allocating descriptor sets
     * @param device Logical device
     * @param descriptorPool Output descriptor pool
     * @details Sized for MAX_FRAMES_IN_FLIGHT descriptor sets with uniform buffer and sampler bindings
     */
    void createDescriptorPool(VkDevice &device, VkDescriptorPool &descriptorPool);

    /**
     * @brief Create and update descriptor sets
     * @param device Logical device
     * @param descriptorPool Pool to allocate from
     * @param descriptorSets Output vector of descriptor sets
     * @param descriptorSetLayout Layout defining bindings
     * @param uniformBuffers Uniform buffers to bind
     * @param textureImageView Texture image view to bind
     * @param textureSampler Texture sampler to bind
     * @details Creates one descriptor set per frame in flight, linking shaders to resources
     */
    void createDescriptorSets(
        VkDevice &device,
        VkDescriptorPool &descriptorPool,
        std::vector<VkDescriptorSet> &descriptorSets,
        VkDescriptorSetLayout &descriptorSetLayout,
        std::vector<VkBuffer> &uniformBuffers,
        VkImageView &textureImageView,
        VkSampler &textureSampler
    );
} // namespace Buffer