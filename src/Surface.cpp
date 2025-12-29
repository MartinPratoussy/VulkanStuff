#include "Instance.hpp"

#include <stdexcept>

namespace Instance
{
    void createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }
} // namespace Instance