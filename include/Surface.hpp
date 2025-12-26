#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <vulkan/vulkan_core.h>

namespace Surface
{
    void createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface);
}