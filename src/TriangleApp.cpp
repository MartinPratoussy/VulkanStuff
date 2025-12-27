#include "TriangleApp.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <cstring>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstddef>
#include <ranges>
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

void TriangleApp::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void TriangleApp::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window =
        glfwCreateWindow(FrameSize::WIDTH, FrameSize::HEIGHT, "Vulkan Triangle", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

static void frameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<TriangleApp *>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void TriangleApp::initVulkan()
{
    Instance::createInstance(instance);

    Surface::createSurface(instance, window, &surface);

    Device::pickPhysicalDevice(instance, physicalDevice, surface);
    Device::checkDeviceExtensionSupport(physicalDevice);
    Device::createLogicalDevice(physicalDevice, device, graphicsQueue, presentQueue, surface);

    SwapChain::createSwapChain(
        physicalDevice, device, surface, swapChain, swapChainImages, swapChainExtent
    );

    ImageViews::
        createImageViews(device, swapChainImages, VK_FORMAT_B8G8R8A8_SRGB, swapChainImageViews);

    GraphicsPipeline::createRenderPass(device, renderPass);

    Buffer::createDescriptorSetLayout(device, descriptorSetLayout);

    GraphicsPipeline::createGraphicsPipeline(
        device, swapChainExtent, pipelineLayout, graphicsPipeline, renderPass, descriptorSetLayout
    );

    Framebuffer::createFramebuffers(
        device, renderPass, swapChainImageViews, swapChainExtent, swapChainFramebuffers
    );

    Command::createCommandPool(device, physicalDevice, surface, commandPool);

    Buffer::createVertexBuffer(
        device, physicalDevice, vertexBuffer, vertexBufferMemory, commandPool, graphicsQueue
    );

    Buffer::createIndexBuffer(
        device, physicalDevice, indexBuffer, indexBufferMemory, commandPool, graphicsQueue
    );

    Buffer::createUniformBuffers(
        device, physicalDevice, uniformBuffers, uniformBuffersMemory, uniformBuffersMapped
    );
    Buffer::createDescriptorPool(device, descriptorPool);
    Buffer::createDescriptorSets(
        device, descriptorPool, descriptorSets, descriptorSetLayout, uniformBuffers
    );

    Command::createCommandBuffers(device, commandPool, commandBuffers, MAX_FRAMES_IN_FLIGHT);

    Synchronization::createSyncObjects(
        device,
        imageAvailableSemaphores,
        renderFinishedSemaphores,
        inFlightFences,
        MAX_FRAMES_IN_FLIGHT
    );
}

void TriangleApp::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

void TriangleApp::drawFrame()
{
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    std::uint32_t imageIndex;
    VkResult resAcquire = vkAcquireNextImageKHR(
        device,
        swapChain,
        UINT64_MAX,
        imageAvailableSemaphores[currentFrame],
        VK_NULL_HANDLE,
        &imageIndex
    );

    if (resAcquire == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }
    else if (resAcquire != VK_SUCCESS && resAcquire != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    updateUniformBuffer(currentFrame);

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

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    VkResult resPresent = vkQueuePresentKHR(presentQueue, &presentInfo);
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

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void TriangleApp::updateUniformBuffer(std::uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime)
                     .count();

    Buffer::Vertex::UniformBufferObject ubo{};
    ubo.model = glm::
        rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.view = glm::lookAt(
        glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
    ubo.proj = glm::perspective(
        glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f
    );
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void TriangleApp::recreateSwapChain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    SwapChain::createSwapChain(
        physicalDevice, device, surface, swapChain, swapChainImages, swapChainExtent
    );
    ImageViews::
        createImageViews(device, swapChainImages, VK_FORMAT_B8G8R8A8_SRGB, swapChainImageViews);
    Framebuffer::createFramebuffers(
        device, renderPass, swapChainImageViews, swapChainExtent, swapChainFramebuffers
    );
}

void TriangleApp::cleanupSwapChain()
{
    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void TriangleApp::cleanup()
{
    cleanupSwapChain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto &&[imageAvailableSemaphore, renderFinishedSemaphore, inFlightFence] :
         std::views::zip(imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences))
    {
        vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
        vkDestroyFence(device, inFlightFence, nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroyDevice(device, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}