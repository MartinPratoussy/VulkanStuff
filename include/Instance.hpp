#pragma once

#include <cstdint>

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Instance
{
	constexpr const char* applicationName = "Vulkan Triangle";
	constexpr const char* engineName = "No Engine";
	constexpr std::uint32_t applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	constexpr std::uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);
	constexpr std::uint32_t apiVersion = VK_API_VERSION_1_0;

	void createInstance(VkInstance& instance);
}