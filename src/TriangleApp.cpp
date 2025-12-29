/**
 * @file TriangleApp.cpp
 * @brief Main Vulkan application implementation with proper synchronization and memory management
 */

#include "TriangleApp.hpp"
#include "Image.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <cstring>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

#include "Buffer.hpp"
#include "Command.hpp"
#include "Device.hpp"
#include "FrameSize.hpp"
#include "Framebuffer.hpp"
#include "GraphicsPipeline.hpp"
#include "ImageViews.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include "Synchronisation.hpp"

/**
 * @brief Main application entry point
 * @details Initializes window, Vulkan resources, runs the rendering loop, and cleans up
 */
void TriangleApp::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

/**
 * @brief Initialize the GLFW window
 * @details Sets up a resizable window with no OpenGL context (Vulkan-only)
 *          and registers framebuffer resize callbacks
 */
void TriangleApp::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); ///< Don't create OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);    ///< Allow window resizing
    window =
        glfwCreateWindow(FrameSize::WIDTH, FrameSize::HEIGHT, "Vulkan Triangle", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this); ///< Store app pointer for callbacks
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

/**
 * @brief Callback when window framebuffer is resized
 * @param window The GLFW window that was resized
 * @param width New framebuffer width (unused but required by GLFW)
 * @param height New framebuffer height (unused but required by GLFW)
 * @details Sets a flag to trigger swapchain recreation on next frame
 */
static void frameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<TriangleApp *>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

/**
 * @brief Initialize all Vulkan resources in the correct order
 * @details Creates instance, devices, swapchain, graphics pipeline, buffers, textures, and synchronization primitives
 * @note Order is critical - dependencies must be created before dependent resources
 */
void TriangleApp::initVulkan()
{
    // Core Vulkan instance and device setup
    Instance::createInstance(instance); ///< Create Vulkan instance with validation layers

    Surface::createSurface(instance, window, &surface); ///< Platform-specific window surface

    Device::pickPhysicalDevice(instance, physicalDevice, surface); ///< Select GPU
    Device::checkDeviceExtensionSupport(physicalDevice);           ///< Verify swapchain support
    Device::createLogicalDevice(physicalDevice, device, graphicsQueue, presentQueue, surface);

    // Swapchain creation (presentation engine)
    SwapChain::createSwapChain(
        physicalDevice, device, surface, swapChain, swapChainImages, swapChainExtent
    );

    // Create image views for each swapchain image
    ImageViews::
        createImageViews(device, swapChainImages, VK_FORMAT_B8G8R8A8_SRGB, swapChainImageViews);

    // Graphics pipeline setup
    GraphicsPipeline::createRenderPass(device, renderPass); ///< Define rendering attachments

    Buffer::createDescriptorSetLayout(device, descriptorSetLayout); ///< Shader resource layout

    GraphicsPipeline::createGraphicsPipeline(
        device, swapChainExtent, pipelineLayout, graphicsPipeline, renderPass, descriptorSetLayout
    );

    // Create framebuffers (one per swapchain image)
    Framebuffer::createFramebuffers(
        device, renderPass, swapChainImageViews, swapChainExtent, swapChainFramebuffers
    );

    // Command buffer infrastructure
    Command::createCommandPool(device, physicalDevice, surface, commandPool);

    // Texture loading and setup
    Image::createTextureImage(
        device, physicalDevice, textureImage, textureImageMemory, commandPool, graphicsQueue
    );

    ImageViews::createTextureImageView(device, textureImage, textureImageView);

    Texture::createTextureSampler(device, physicalDevice, textureSampler); ///< Texture filtering

    // Vertex and index buffer creation
    Buffer::createVertexBuffer(
        device, physicalDevice, vertexBuffer, vertexBufferMemory, commandPool, graphicsQueue
    );

    Buffer::createIndexBuffer(
        device, physicalDevice, indexBuffer, indexBufferMemory, commandPool, graphicsQueue
    );

    // Uniform buffer setup (per frame in flight for dynamic updates)
    Buffer::createUniformBuffers(
        device, physicalDevice, uniformBuffers, uniformBuffersMemory, uniformBuffersMapped
    );

    // Descriptor pool and sets (links shaders to resources)
    Buffer::createDescriptorPool(device, descriptorPool);
    Buffer::createDescriptorSets(
        device,
        descriptorPool,
        descriptorSets,
        descriptorSetLayout,
        uniformBuffers,
        textureImageView,
        textureSampler
    );

    // Command buffers for recording draw commands
    Command::createCommandBuffers(device, commandPool, commandBuffers, MAX_FRAMES_IN_FLIGHT);

    /**
     * Synchronization primitives:
     * - imageAvailableSemaphores: One per frame in flight (used for acquire)
     * - renderFinishedSemaphores: One per swapchain image (prevents reuse before present)
     * - inFlightFences: CPU-GPU sync for frame pacing
     */
    Synchronization::createSyncObjects(
        device,
        imageAvailableSemaphores,
        renderFinishedSemaphores,
        inFlightFences,
        swapChainImages.size(),
        MAX_FRAMES_IN_FLIGHT
    );
}

/**
 * @brief Main rendering loop
 * @details Continuously processes events and renders frames until window closes
 * @note Waits for device to finish all operations before exiting
 */
void TriangleApp::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); ///< Process window events (resize, close, etc.)
        drawFrame();      ///< Render a single frame
    }

    vkDeviceWaitIdle(device); ///< Wait for all GPU operations to complete
}

/**
 * @brief Render a single frame
 * @details Implements frame-in-flight rendering with proper synchronization:
 *          1. Wait for previous frame using this slot to finish
 *          2. Acquire next swapchain image
 *          3. Update uniform buffers (animation)
 *          4. Record command buffer
 *          5. Submit to GPU
 *          6. Present to screen
 * @note Uses separate semaphores per frame (acquire) and per image (present) to avoid validation errors
 */
void TriangleApp::drawFrame()
{
    /// Wait for the previous frame using this slot to finish rendering
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    /// Acquire the next available swapchain image
    std::uint32_t imageIndex;
    VkResult resAcquire = vkAcquireNextImageKHR(
        device,
        swapChain,
        UINT64_MAX,
        imageAvailableSemaphores[currentFrame], ///< Signaled when image is available
        VK_NULL_HANDLE,
        &imageIndex
    );

    /// Handle swapchain out of date (window resize)
    if (resAcquire == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }
    else if (resAcquire != VK_SUCCESS && resAcquire != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    /// Reset fence only after successfully acquiring image
    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    /// Update transformation matrices for animation
    updateUniformBuffer(currentFrame);

    /// Record rendering commands into command buffer
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    Command::recordCommandBuffer(
        commandBuffers[currentFrame],
        renderPass,
        swapChainFramebuffers[imageIndex],
        swapChainExtent,
        graphicsPipeline,
        pipelineLayout,
        vertexBuffer,
        indexBuffer,
        descriptorSets,
        currentFrame
    );

    /// Submit command buffer to GPU
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    /// Wait for image to be available before writing colors
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    /// Signal per-image semaphore when rendering is done (critical for preventing reuse)
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[imageIndex]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    /// Present the rendered image to the screen
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores; ///< Wait for rendering to finish

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    VkResult resPresent = vkQueuePresentKHR(presentQueue, &presentInfo);

    /// Handle window resize or suboptimal swapchain
    if (resPresent == VK_ERROR_OUT_OF_DATE_KHR || resPresent == VK_SUBOPTIMAL_KHR
        || framebufferResized)
    {
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (resPresent != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    /// Advance to next frame slot (wraps around)
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

/**
 * @brief Update uniform buffer with current transformation matrices
 * @param currentImage Index of the frame's uniform buffer to update
 * @details Creates animated rotation by updating model matrix based on elapsed time.
 *          Uses persistently mapped memory for efficient updates every frame.
 *          View and projection matrices remain static.
 * @note proj[1][1] is negated to flip Y-axis for Vulkan's coordinate system
 */
void TriangleApp::updateUniformBuffer(std::uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    /// Calculate elapsed time since app started
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime)
                     .count();

    /// Build transformation matrices
    Buffer::Vertex::UniformBufferObject ubo{};

    /// Model matrix: rotate 90 degrees per second around Z-axis
    ubo.model = glm::
        rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    /// View matrix: camera positioned at (2,2,2) looking at origin
    ubo.view = glm::lookAt(
        glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );

    /// Projection matrix: 45° FOV perspective
    ubo.proj = glm::perspective(
        glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f
    );
    ubo.proj[1][1] *= -1; ///< Flip Y for Vulkan (GLM uses OpenGL conventions)

    /// Copy to mapped GPU memory (no need to map/unmap each frame)
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

/**
 * @brief Recreate swapchain after window resize or invalidation
 * @details Handles window minimization, waits for valid size, destroys old resources,
 *          and creates new swapchain with updated dimensions
 * @note Must recreate renderFinished semaphores as swapchain image count may change
 */
void TriangleApp::recreateSwapChain()
{
    /// Handle window minimization - wait until window has valid size
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    /// Wait for all GPU operations to complete before destroying resources
    vkDeviceWaitIdle(device);

    /// Clean up old swapchain and dependent resources
    cleanupSwapChain();

    /// Reset handles to ensure clean state
    swapChain = VK_NULL_HANDLE;
    swapChainImages.clear();
    swapChainImageViews.clear();
    swapChainFramebuffers.clear();

    /// Recreate swapchain with new dimensions
    SwapChain::createSwapChain(
        physicalDevice, device, surface, swapChain, swapChainImages, swapChainExtent
    );
    ImageViews::
        createImageViews(device, swapChainImages, VK_FORMAT_B8G8R8A8_SRGB, swapChainImageViews);
    Framebuffer::createFramebuffers(
        device, renderPass, swapChainImageViews, swapChainExtent, swapChainFramebuffers
    );

    /**
     * Recreate renderFinished semaphores since swapchain image count may have changed
     * imageAvailable semaphores and fences don't need recreation (frame-based, not image-based)
     */
    Synchronization::createSyncObjects(
        device,
        imageAvailableSemaphores,
        renderFinishedSemaphores,
        inFlightFences,
        swapChainImages.size(),
        MAX_FRAMES_IN_FLIGHT
    );
}

/**
 * @brief Clean up swapchain-dependent resources
 * @details Destroys framebuffers, image views, swapchain, and per-image semaphores.
 *          Called during swapchain recreation and final cleanup.
 * @note Does NOT destroy imageAvailable semaphores (those are frame-based, not image-based)
 */
void TriangleApp::cleanupSwapChain()
{
    /// Destroy framebuffers (one per swapchain image)
    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    /// Destroy image views (we don't own the images themselves)
    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
    }

    /// Destroy the swapchain itself
    vkDestroySwapchainKHR(device, swapChain, nullptr);

    /// Destroy renderFinished semaphores (indexed by swapchain image, may change on resize)
    for (auto semaphore : renderFinishedSemaphores)
    {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
    renderFinishedSemaphores.clear();
}

/**
 * @brief Clean up all Vulkan resources in reverse order of creation
 * @details Destroys resources in reverse dependency order to prevent validation errors.
 *          Ensures all memory is properly freed and unmapped.
 * @note Order is critical - dependent resources must be destroyed before their dependencies
 */
void TriangleApp::cleanup()
{
    /// Clean up swapchain and per-image resources first
    cleanupSwapChain();

    /// Texture resources
    vkDestroySampler(device, textureSampler, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);
    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);

    /// Uniform buffers (unmap before destroying to prevent memory leak)
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkUnmapMemory(
            device, uniformBuffersMemory[i]
        ); ///< CRITICAL: unmap persistently mapped memory
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

    /// Descriptor pool (automatically frees descriptor sets)
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    /// Vertex and index buffers
    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);

    /// Graphics pipeline and layout
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    /// Synchronization primitives (frame-based)
    for (auto semaphore : imageAvailableSemaphores)
    {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
    for (auto fence : inFlightFences)
    {
        vkDestroyFence(device, fence, nullptr);
    }

    /// Command pool (automatically frees command buffers)
    vkDestroyCommandPool(device, commandPool, nullptr);

    /// Device and instance
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    /// GLFW cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
}