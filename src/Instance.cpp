#include "Instance.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

#include "ValidationLayers.hpp"

namespace Instance
{
    void createInstance(VkInstance &instance)
    {
        if (ValidationLayers::enableValidationLayers
            && !ValidationLayers::checkValidationLayerSupport())
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName;
        appInfo.applicationVersion = applicationVersion;
        appInfo.pEngineName = engineName;
        appInfo.engineVersion = engineVersion;
        appInfo.apiVersion = apiVersion;

        uint32_t extensionCount = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = extensionCount;
        createInfo.ppEnabledExtensionNames = extensions;

        if (ValidationLayers::enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<std::uint32_t>(
                ValidationLayers::validationLayers.size()
            );
            createInfo.ppEnabledLayerNames = ValidationLayers::validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
    }
} // namespace Instance