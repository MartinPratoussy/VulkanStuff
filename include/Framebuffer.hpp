#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

namespace Framebuffer
{
	void createFramebuffers(VkDevice& device, VkRenderPass& renderPass, const std::vector<VkImageView>& swapChainImageViews, VkExtent2D& swapChainExtent, std::vector<VkFramebuffer>& swapChainFramebuffers);
} // namespace Framebuffer