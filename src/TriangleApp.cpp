#include "TriangleApp.hpp"

#include <stdexcept>
#include <map>

#include "Device.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include "FrameSize.hpp"
#include "Instance.hpp"
#include "ImageViews.hpp"
#include "GraphicsPipeline.hpp"
#include "Framebuffer.hpp"
#include "Command.hpp"
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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(FrameSize::WIDTH, FrameSize::HEIGHT, "Vulkan Triangle", nullptr, nullptr);
}

void TriangleApp::initVulkan()
{
	Instance::createInstance(instance);

	Surface::createSurface(instance, window, &surface);

	Device::pickPhysicalDevice(instance, physicalDevice, surface);
	Device::checkDeviceExtensionSupport(physicalDevice);
	Device::createLogicalDevice(physicalDevice, device, graphicsQueue, presentQueue, surface);

	SwapChain::createSwapChain(physicalDevice, device, surface, swapChain, swapChainImages, swapChainExtent);

	ImageViews::createImageViews(device, swapChainImages, VK_FORMAT_B8G8R8A8_SRGB, swapChainImageViews);

	GraphicsPipeline::createRenderPass(device, renderPass);
	GraphicsPipeline::createGraphicsPipeline(device, swapChainExtent, pipelineLayout, graphicsPipeline, renderPass);

	Framebuffer::createFramebuffers(device, renderPass, swapChainImageViews, swapChainExtent, swapChainFramebuffers);

	Command::createCommandPool(device, physicalDevice, surface, commandPool);
	Command::createCommandBuffer(device, commandPool, commandBuffer);

	Synchronization::createSyncObjects(device, imageAvailableSemaphore, renderFinishedSemaphore, inFlightFence);
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
	vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &inFlightFence);

	std::uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(commandBuffer, 0);
	Command::recordCommandBuffer(commandBuffer, renderPass, swapChainFramebuffers[imageIndex], swapChainExtent, graphicsPipeline);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	vkQueuePresentKHR(presentQueue, &presentInfo);
}

void TriangleApp::cleanup()
{
	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroyFence(device, inFlightFence, nullptr);
	vkDestroyCommandPool(device, commandPool, nullptr);
	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}