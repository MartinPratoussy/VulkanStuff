/**
 * @file GraphicsPipeline.hpp
 * @brief Graphics pipeline and shader management
 */

#pragma once

#include <string_view>
#include <vector>
#include <vulkan/vulkan.h>

/**
 * @namespace GraphicsPipeline
 * @brief Creates graphics pipeline, shaders, and render pass
 */
namespace GraphicsPipeline
{
    /// Path to compiled vertex shader (SPIR-V)
    constexpr std::string_view vertShaderPath = "build/shaders/shader.vert.spv";
    /// Path to compiled fragment shader (SPIR-V)
    constexpr std::string_view fragShaderPath = "build/shaders/shader.frag.spv";

    /**
     * @brief Create graphics pipeline with shaders and fixed-function state
     * @param device Logical device
     * @param swapChainExtent Viewport/scissor dimensions
     * @param pipelineLayout Output pipeline layout
     * @param graphicsPipeline Output graphics pipeline
     * @param renderPass Render pass for pipeline
     * @param descriptorSetLayout Descriptor set layout for resources
     * @details Configures vertex input, rasterization, depth/stencil, blending
     */
    void createGraphicsPipeline(
        VkDevice &device,
        VkExtent2D &swapChainExtent,
        VkPipelineLayout &pipelineLayout,
        VkPipeline &graphicsPipeline,
        VkRenderPass &renderPass,
        VkDescriptorSetLayout &descriptorSetLayout
    );

    /**
     * @brief Create shader module from SPIR-V bytecode
     * @param device Logical device
     * @param code SPIR-V shader bytecode
     * @return Shader module handle
     * @details Wraps compiled shader code for pipeline usage
     */
    VkShaderModule createShaderModule(VkDevice &device, const std::vector<std::byte> &code);

    /**
     * @brief Create render pass defining attachments and subpasses
     * @param device Logical device
     * @param renderPass Output render pass handle
     * @details Defines single color attachment with clear on load, store on finish
     */
    void createRenderPass(VkDevice &device, VkRenderPass &renderPass);
} // namespace GraphicsPipeline