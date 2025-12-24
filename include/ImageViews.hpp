#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

namespace ImageViews
{
	void createImageViews(VkDevice &device, const std::vector<VkImage> &swapChainImages, VkFormat swapChainImageFormat, std::vector<VkImageView> &swapChainImageViews);
}