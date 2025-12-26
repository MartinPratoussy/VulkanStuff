#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace SwapChain
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice &device, VkSurfaceKHR &surface);
    VkSurfaceFormatKHR
    chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR
    chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    void createSwapChain(
        VkPhysicalDevice &physicalDevice,
        VkDevice &device,
        VkSurfaceKHR &surface,
        VkSwapchainKHR &swapChain,
        std::vector<VkImage> &swapChainImages,
        VkExtent2D &swapChainExtent
    );
} // namespace SwapChain