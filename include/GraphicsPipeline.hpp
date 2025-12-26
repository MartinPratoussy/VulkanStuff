#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace GraphicsPipeline
{
    constexpr std::string_view vertShaderPath = "build/shaders/shader.vert.spv";
    constexpr std::string_view fragShaderPath = "build/shaders/shader.frag.spv";

    void createGraphicsPipeline(
        VkDevice &device,
        VkExtent2D &swapChainExtent,
        VkPipelineLayout &pipelineLayout,
        VkPipeline &graphicsPipeline,
        VkRenderPass &renderPass
    );
    VkShaderModule createShaderModule(VkDevice &device, const std::vector<std::byte> &code);
    void createRenderPass(VkDevice &device, VkRenderPass &renderPass);
} // namespace GraphicsPipeline