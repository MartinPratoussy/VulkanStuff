#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

namespace Command
{
	void createCommandPool(VkDevice& device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, VkCommandPool& commandPool);
	void createCommandBuffer(VkDevice& device, VkCommandPool& commandPool, VkCommandBuffer& commandBuffer);
	void recordCommandBuffer(VkCommandBuffer& commandBuffer, VkRenderPass& renderPass, VkFramebuffer& framebuffer, VkExtent2D& extent, VkPipeline& graphicsPipeline);
} // namespace CommandPool