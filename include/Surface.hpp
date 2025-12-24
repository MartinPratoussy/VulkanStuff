#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan_core.h>

namespace Surface
{
	void createSurface(VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface);
}