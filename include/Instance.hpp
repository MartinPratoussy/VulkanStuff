/**
 * @file Instance.hpp
 * @brief Vulkan instance creation with application info
 */

#pragma once

#include <cstdint>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan_core.h>

/**
 * @namespace Instance
 * @brief Creates Vulkan instance with validation layers
 */
namespace Instance
{
    constexpr const char *applicationName = "Vulkan Triangle"; ///< Application name
    constexpr const char *engineName = "No Engine";            ///< Engine name (none used)
    constexpr std::uint32_t applicationVersion = VK_MAKE_VERSION(1, 0, 0); ///< App version
    constexpr std::uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);      ///< Engine version
    constexpr std::uint32_t apiVersion = VK_API_VERSION_1_0; ///< Target Vulkan API version

    /**
     * @brief Create Vulkan instance
     * @param instance Output instance handle
     * @details Creates instance with GLFW extensions and validation layers (debug builds)
     */
    void createInstance(VkInstance &instance);

    /**
     * @brief Create window surface for rendering
     * @param instance Vulkan instance
     * @param window GLFW window handle
     * @param surface Output surface handle
     * @details Uses platform-specific extensions (Win32, X11, Wayland, etc.) via GLFW
     */
    void createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface);
} // namespace Instance