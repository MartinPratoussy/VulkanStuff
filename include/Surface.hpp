/**
 * @file Surface.hpp
 * @brief Platform-specific window surface creation
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <vulkan/vulkan_core.h>

/**
 * @namespace Surface
 * @brief Creates platform-specific Vulkan surface for presentation
 */
namespace Surface
{
    /**
     * @brief Create window surface for rendering
     * @param instance Vulkan instance
     * @param window GLFW window handle
     * @param surface Output surface handle
     * @details Uses platform-specific extensions (Win32, X11, Wayland, etc.) via GLFW
     */
    void createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface);
} // namespace Surface