#pragma once

#include <vulkan/vulkan_core.h>
#include <optional>
#include <cstdint>

namespace Queue
{
	struct FamilyIndices
	{
		std::optional<std::uint32_t> graphicsFamily;
		std::optional<std::uint32_t> presentFamily;

		bool isComplete() const
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	FamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
}