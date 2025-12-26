#include "Device.hpp"

#include "Queue.hpp"
#include "ValidationLayers.hpp"
#include <map>
#include <set>
#include <stdexcept>
#include <vector>

namespace Device
{
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    void pickPhysicalDevice(
        VkInstance &instance, VkPhysicalDevice &physicalDevice, VkSurfaceKHR &surface
    )
    {
        std::uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        std::multimap<std::uint32_t, VkPhysicalDevice> candidates;

        for (auto device : devices)
        {
            auto rate = rateDevice(device, surface);
            candidates.insert(std::make_pair(rate, device));

            if (candidates.rbegin()->first > 0)
            {
                physicalDevice = candidates.rbegin()->second;
            }
            else
            {
                throw std::runtime_error("failed to find a suitable GPU!");
            }
        }
    }

    std::uint32_t rateDevice(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        Queue::FamilyIndices indices = Queue::findQueueFamilies(device, surface);

        if (!indices.graphicsFamily.has_value() || !indices.isComplete())
        {
            return 0;
        }

        std::uint32_t score = 0;

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        score += deviceProperties.limits.maxImageDimension2D;

        if (!deviceFeatures.geometryShader)
        {
            return 0;
        }

        return score;
    }

    void createLogicalDevice(
        VkPhysicalDevice physicalDevice,
        VkDevice &device,
        VkQueue &graphicsQueue,
        VkQueue &presentQueue,
        VkSurfaceKHR surface
    )
    {
        Queue::FamilyIndices indices = Queue::findQueueFamilies(physicalDevice, surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<std::uint32_t> uniqueQueueFamilies =
            {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (std::uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<std::uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

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

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice &device)
    {
        std::uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(
            device, nullptr, &extensionCount, availableExtensions.data()
        );

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto &extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
} // namespace Device