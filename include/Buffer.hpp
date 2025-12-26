#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Buffer
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    const std::vector<Vertex> vertices =
        {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
         {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
         {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
         {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

    const std::vector<std::uint16_t> indices = {0, 1, 2, 2, 3, 0};

    void createVertexBuffer(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkBuffer &vertexBuffer,
        VkDeviceMemory &vertexBufferMemory,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

    void createIndexBuffer(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkBuffer &vertexBuffer,
        VkDeviceMemory &vertexBufferMemory,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

    void createBuffer(
        VkDevice &device,
        VkPhysicalDevice &physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer &vertexBuffer,
        VkDeviceMemory &vertexBufferMemory
    );

    void copyBuffer(
        VkDevice &device,
        VkBuffer srcBuffer,
        VkBuffer dstBuffer,
        VkDeviceSize size,
        VkCommandPool &commandPool,
        VkQueue &graphicsQueue
    );

    std::uint32_t findMemoryType(
        VkPhysicalDevice &physicalDevice, std::uint32_t typeFilter, VkMemoryPropertyFlags properties
    );

} // namespace Buffer