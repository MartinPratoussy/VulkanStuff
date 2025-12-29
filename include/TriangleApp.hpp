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
 * @struct VulkanCore
 * @brief Core Vulkan objects required for rendering
 * @details Groups the fundamental Vulkan objects that are created early and
 *          used throughout the application's lifetime
 */
struct VulkanCore
{
    VkInstance instance = VK_NULL_HANDLE;             ///< Vulkan instance (connection to Vulkan)
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; ///< Selected GPU
    VkDevice device = VK_NULL_HANDLE;                 ///< Logical device (interface to GPU)
    VkQueue graphicsQueue = VK_NULL_HANDLE;           ///< Queue for graphics commands
    VkQueue presentQueue = VK_NULL_HANDLE;            ///< Queue for presentation
    VkSurfaceKHR surface = VK_NULL_HANDLE;            ///< Window surface for rendering
};

/**
 * @struct SwapchainResources
 * @brief Swapchain and dependent resources that must be recreated on resize
 * @details Groups all resources that depend on swapchain dimensions and must
 *          be destroyed/recreated when the window is resized
 */
struct SwapchainResources
{
    VkSwapchainKHR swapChain = VK_NULL_HANDLE; ///< Swapchain (image presentation engine)
    VkExtent2D extent = {};                    ///< Resolution of swapchain images
    std::vector<VkImage> images;               ///< Swapchain images (owned by swapchain)
    std::vector<VkImageView> imageViews;       ///< Image views for swapchain images
    std::vector<VkFramebuffer> framebuffers;   ///< Framebuffers (one per swapchain image)
};

/**
 * @struct PipelineResources
 * @brief Graphics pipeline and related objects
 * @details Groups the graphics pipeline and its dependencies
 */
struct PipelineResources
{
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE; ///< Layout for shader resources
    VkPipelineLayout layout = VK_NULL_HANDLE; ///< Pipeline layout (uniforms, push constants)
    VkRenderPass renderPass = VK_NULL_HANDLE; ///< Render pass (attachments and subpasses)
    VkPipeline pipeline = VK_NULL_HANDLE;     ///< Graphics pipeline (shaders and state)
};

/**
 * @struct BufferResources
 * @brief Vertex, index, and uniform buffers with their backing memory
 * @details Groups all buffer objects and their associated device memory
 */
struct BufferResources
{
    VkBuffer vertexBuffer = VK_NULL_HANDLE;       ///< GPU buffer for vertex data
    VkDeviceMemory vertexMemory = VK_NULL_HANDLE; ///< Memory backing vertex buffer
    VkBuffer indexBuffer = VK_NULL_HANDLE;        ///< GPU buffer for index data
    VkDeviceMemory indexMemory = VK_NULL_HANDLE;  ///< Memory backing index buffer

    std::vector<VkBuffer> uniformBuffers;      ///< Uniform buffers for transformation matrices
    std::vector<VkDeviceMemory> uniformMemory; ///< Memory backing uniform buffers
    std::vector<void *> uniformMapped;         ///< Persistently mapped pointers for updates
};

/**
 * @struct TextureResources
 * @brief Texture image, view, and sampler
 * @details Groups all texture-related resources
 */
struct TextureResources
{
    VkImage image = VK_NULL_HANDLE;         ///< Texture image on GPU
    VkDeviceMemory memory = VK_NULL_HANDLE; ///< Memory backing texture image
    VkImageView view = VK_NULL_HANDLE;      ///< Image view for texture
    VkSampler sampler = VK_NULL_HANDLE;     ///< Sampler (filtering and addressing)
};

/**
 * @struct SyncResources
 * @brief Synchronization primitives for frame coordination
 * @details Groups semaphores and fences with proper documentation of indexing
 */
struct SyncResources
{
    /**
     * Semaphores indexed by currentFrame (0 to MAX_FRAMES_IN_FLIGHT-1)
     * Used for vkAcquireNextImageKHR to signal when image is available
     */
    std::vector<VkSemaphore> imageAvailable;

    /**
     * Semaphores indexed by imageIndex (0 to swapChainImages.size()-1)
     * Used in vkQueueSubmit to signal when rendering is finished
     * CRITICAL: Prevents reuse of semaphore before swapchain image is re-acquired
     */
    std::vector<VkSemaphore> renderFinished;

    /**
     * Fences indexed by currentFrame for CPU-GPU synchronization
     * Ensures a frame slot isn't reused until GPU finishes with it
     */
    std::vector<VkFence> inFlight;
};

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

    // === Resource Groups ===

    GLFWwindow *window = nullptr; ///< GLFW window handle

    VulkanCore vulkan;            ///< Core Vulkan objects (instance, device, queues)
    SwapchainResources swapchain; ///< Swapchain and dependent resources
    PipelineResources pipeline;   ///< Graphics pipeline and layout
    BufferResources buffers;      ///< Vertex, index, and uniform buffers
    TextureResources texture;     ///< Texture image, view, and sampler
    SyncResources sync;           ///< Synchronization primitives

    VkCommandPool commandPool = VK_NULL_HANDLE;  ///< Command pool for allocating command buffers
    std::vector<VkCommandBuffer> commandBuffers; ///< Command buffers (one per frame in flight)

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE; ///< Pool for allocating descriptor sets
    std::vector<VkDescriptorSet> descriptorSets; ///< Descriptor sets (bind resources to shaders)

    std::uint32_t currentFrame = 0; ///< Current frame index (wraps around)
};