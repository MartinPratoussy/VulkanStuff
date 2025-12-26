#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ValidationLayers
{
    extern const std::vector<const char *> validationLayers;
    extern const bool enableValidationLayers;

    bool checkValidationLayerSupport();
} // namespace ValidationLayers