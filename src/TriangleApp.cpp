#include "TriangleApp.hpp"

#include <ranges>
#include <stdexcept>

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
#include "Vertex.hpp"

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
    GraphicsPipeline::createGraphicsPipeline(
        device, swapChainExtent, pipelineLayout, graphicsPipeline, renderPass
    );

    Framebuffer::createFramebuffers(
        device, renderPass, swapChainImageViews, swapChainExtent, swapChainFramebuffers
    );

    Command::createCommandPool(device, physicalDevice, surface, commandPool);

    Vertex::createVertexBuffer(device, physicalDevice, vertexBuffer, vertexBufferMemory);

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

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    Command::recordCommandBuffer(
        commandBuffers[currentFrame],
        renderPass,
        swapChainFramebuffers[imageIndex],
        swapChainExtent,
        graphicsPipeline,
        vertexBuffer
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