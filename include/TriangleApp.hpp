/**
 * @file TriangleApp.hpp
 * @brief Main Vulkan application class with proper frame-in-flight rendering
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

/// Maximum number of frames that can be processed concurrently on the GPU
constexpr std::uint8_t MAX_FRAMES_IN_FLIGHT = 2;

/// Callback function for window framebuffer resize events
static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);

/**
 * @class TriangleApp
 * @brief Main Vulkan application managing rendering, resources, and synchronization
 * @details Implements a complete Vulkan rendering pipeline with:
 *          - Proper frame-in-flight synchronization
 *          - Texture mapping
 *          - Animated transformations
 *          - Swapchain recreation on window resize
 *          - Clean resource management without memory leaks
 */
class TriangleApp
{
  public:
    /**
     * @brief Main application entry point
     * @details Initializes, runs main loop, and cleans up
     */
    void run();

    /// Flag set by resize callback to trigger swapchain recreation
    bool framebufferResized = false;

  private:
    // === Initialization Methods ===

    /**
     * @brief Initialize GLFW window
     * @details Creates a resizable window without OpenGL context
     */
    void initWindow();

    /**
     * @brief Initialize all Vulkan resources
     * @details Creates instance, devices, swapchain, pipeline, buffers, and sync objects
     */
    void initVulkan();

    // === Rendering Methods ===

    /**
     * @brief Main rendering loop
     * @details Processes events and renders frames until window closes
     */
    void mainLoop();

    /**
     * @brief Render a single frame
     * @details Acquires image, updates uniforms, records commands, submits, and presents
     */
    void drawFrame();

    /**
     * @brief Update uniform buffer with transformation matrices
     * @param currentImage Index of uniform buffer to update
     * @details Updates model, view, and projection matrices for animation
     */
    void updateUniformBuffer(std::uint32_t currentImage);

    // === Resource Management ===

    /**
     * @brief Recreate swapchain after window resize
     * @details Destroys old swapchain and creates new one with updated dimensions
     */
    void recreateSwapChain();

    /**
     * @brief Clean up swapchain-dependent resources
     * @details Destroys framebuffers, image views, swapchain, and per-image semaphores
     */
    void cleanupSwapChain();

    /**
     * @brief Clean up all Vulkan and GLFW resources
     * @details Destroys resources in reverse order of creation to prevent leaks
     */
    void cleanup();

    // === Vulkan Core Objects ===

    GLFWwindow *window = nullptr;                     ///< GLFW window handle
    VkInstance instance = VK_NULL_HANDLE;             ///< Vulkan instance (connection to Vulkan)
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; ///< Selected GPU
    VkDevice device = VK_NULL_HANDLE;                 ///< Logical device (interface to GPU)
    VkQueue graphicsQueue = VK_NULL_HANDLE;           ///< Queue for graphics commands
    VkQueue presentQueue = VK_NULL_HANDLE;            ///< Queue for presentation
    VkSurfaceKHR surface = VK_NULL_HANDLE;            ///< Window surface for rendering

    // === Swapchain Resources ===

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;        ///< Swapchain (image presentation engine)
    VkExtent2D swapChainExtent = {};                  ///< Resolution of swapchain images
    std::vector<VkImage> swapChainImages;             ///< Swapchain images (owned by swapchain)
    std::vector<VkImageView> swapChainImageViews;     ///< Image views for swapchain images
    std::vector<VkFramebuffer> swapChainFramebuffers; ///< Framebuffers (one per swapchain image)

    // === Graphics Pipeline ===

    VkDescriptorSetLayout descriptorSetLayout; ///< Layout for shader resources
    VkPipelineLayout
        pipelineLayout = VK_NULL_HANDLE;          ///< Pipeline layout (uniforms, push constants)
    VkRenderPass renderPass = VK_NULL_HANDLE;     ///< Render pass (attachments and subpasses)
    VkPipeline graphicsPipeline = VK_NULL_HANDLE; ///< Graphics pipeline (shaders and state)

    // === Command Buffers ===

    VkCommandPool commandPool = VK_NULL_HANDLE;  ///< Command pool for allocating command buffers
    std::vector<VkCommandBuffer> commandBuffers; ///< Command buffers (one per frame in flight)

    // === Synchronization Primitives ===

    /**
     * Semaphores indexed by currentFrame (0 to MAX_FRAMES_IN_FLIGHT-1)
     * Used for vkAcquireNextImageKHR to signal when image is available
     */
    std::vector<VkSemaphore> imageAvailableSemaphores;

    /**
     * Semaphores indexed by imageIndex (0 to swapChainImages.size()-1)
     * Used in vkQueueSubmit to signal when rendering is finished
     * CRITICAL: Prevents reuse of semaphore before swapchain image is re-acquired
     */
    std::vector<VkSemaphore> renderFinishedSemaphores;

    /**
     * Fences indexed by currentFrame for CPU-GPU synchronization
     * Ensures a frame slot isn't reused until GPU finishes with it
     */
    std::vector<VkFence> inFlightFences;

    std::uint32_t currentFrame = 0; ///< Current frame index (wraps around)

    // === Vertex Data ===

    VkBuffer vertexBuffer = VK_NULL_HANDLE;             ///< GPU buffer for vertex data
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE; ///< Memory backing vertex buffer
    VkBuffer indexBuffer = VK_NULL_HANDLE;              ///< GPU buffer for index data
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;  ///< Memory backing index buffer

    // === Uniform Buffers (per frame in flight) ===

    std::vector<VkBuffer> uniformBuffers; ///< Uniform buffers for transformation matrices
    std::vector<VkDeviceMemory> uniformBuffersMemory; ///< Memory backing uniform buffers
    std::vector<void *> uniformBuffersMapped;         ///< Persistently mapped pointers for updates

    // === Descriptors ===

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE; ///< Pool for allocating descriptor sets
    std::vector<VkDescriptorSet> descriptorSets; ///< Descriptor sets (bind resources to shaders)

    // === Texture Resources ===

    VkImage textureImage = VK_NULL_HANDLE;              ///< Texture image on GPU
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE; ///< Memory backing texture image
    VkImageView textureImageView = VK_NULL_HANDLE;      ///< Image view for texture
    VkSampler textureSampler = VK_NULL_HANDLE;          ///< Sampler (filtering and addressing)
};