/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Vulkan backend for the engine.
 * Wraps Vulkan handles and provides access to them.
 * 
 * TODO:
 *  - Flesh out the class (VulkanCore) and its methods, structs (before it, also inside of hd namespace)
 *  - Do the above without removing the existing imgui_impl_vulkan code in VulkanCore.cpp. To gradually replace it with our own implementation.
 *  - Add logging - make use of Vulkan's validation layers
 *  - Add support for customizing the implementation of imgui_impl_vulkan.cpp
 */
#pragma once
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include <vulkan/vulkan.h>

#include "imgui.h"
#include "vk_mem_alloc.h"

// 1. First, ensure NO_PROTOTYPES is defined
//#ifndef VK_NO_PROTOTYPES
//#define VK_NO_PROTOTYPES
//#endif

// 2. Tell ImGui we'll handle function loading
//#define IMGUI_IMPL_VULKAN_NO_PROTOTYPES

// 3. Force ImGui to use Volk
//#define IMGUI_IMPL_VULKAN_USE_VOLK

// 4. Include Volk before anything else
//#include "volk.h"

// Current version of the backend use 1 descriptor for the font atlas + as many as additional calls done to ImGui_ImplVulkan_AddTexture().
// It is expected that as early as Q1 2025 the backend will use a few more descriptors. Use this value + number of desired calls to ImGui_ImplVulkan_AddTexture().
#define IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE   (1)     // Minimum per atlas

// =========================== Legacy Vulkan Backend, by ImGui ============================

// Initialization data, for ImGui_ImplVulkan_Init()
// [Please zero-clear before use!]
// - About descriptor pool:
//   - A VkDescriptorPool should be created with VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
//     and must contain a pool size large enough to hold a small number of VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER descriptors.
//   - As an convenience, by setting DescriptorPoolSize > 0 the backend will create one for you.
// - About dynamic rendering:
//   - When using dynamic rendering, set UseDynamicRendering=true and fill PipelineRenderingCreateInfo structure.
struct ImGui_ImplVulkan_InitInfo
{
    VkInstance                      Instance;
    VkPhysicalDevice                PhysicalDevice;
    VkDevice                        Device;
    uint32_t                        QueueFamily;
    VkQueue                         Queue;
    VkDescriptorPool                DescriptorPool;               // See requirements in note above; ignored if using DescriptorPoolSize > 0
    VkRenderPass                    RenderPass;                   // Ignored if using dynamic rendering
    uint32_t                        MinImageCount;                // >= 2
    uint32_t                        ImageCount;                   // >= MinImageCount
    VkSampleCountFlagBits           MSAASamples;                  // 0 defaults to VK_SAMPLE_COUNT_1_BIT

    // (Optional)
    VkPipelineCache                 PipelineCache;
    uint32_t                        Subpass;

    // (Optional) Set to create internal descriptor pool instead of using DescriptorPool
    uint32_t                        DescriptorPoolSize;

    // (Optional) Dynamic Rendering
    // Need to explicitly enable VK_KHR_dynamic_rendering extension to use this, even for Vulkan 1.3.
    bool                            UseDynamicRendering;
#ifdef IMGUI_IMPL_VULKAN_HAS_DYNAMIC_RENDERING
    VkPipelineRenderingCreateInfoKHR PipelineRenderingCreateInfo;
#endif

    // (Optional) Allocation, Debugging
    const VkAllocationCallbacks*    Allocator;
    void                            (*CheckVkResultFn)(VkResult err);
    VkDeviceSize                    MinAllocationSize;      // Minimum allocation size. Set to 1024*1024 to satisfy zealous best practices validation layer and waste a little memory.

    // VMA allocator instance
    VmaAllocator                    VmaAllocator;
};

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
IMGUI_IMPL_API bool             ImGui_ImplVulkan_Init(ImGui_ImplVulkan_InitInfo* info);
IMGUI_IMPL_API void             ImGui_ImplVulkan_Shutdown();
IMGUI_IMPL_API void             ImGui_ImplVulkan_NewFrame();
IMGUI_IMPL_API void             ImGui_ImplVulkan_RenderDrawData(ImDrawData* draw_data, VkCommandBuffer command_buffer, VkPipeline pipeline = VK_NULL_HANDLE);
IMGUI_IMPL_API bool             ImGui_ImplVulkan_CreateFontsTexture();
IMGUI_IMPL_API void             ImGui_ImplVulkan_DestroyFontsTexture();
IMGUI_IMPL_API void             ImGui_ImplVulkan_SetMinImageCount(uint32_t min_image_count); // To override MinImageCount after initialization (e.g. if swap chain is recreated)

// Register a texture (VkDescriptorSet == ImTextureID)
// FIXME: This is experimental in the sense that we are unsure how to best design/tackle this problem
// Please post to https://github.com/ocornut/imgui/pull/914 if you have suggestions.
IMGUI_IMPL_API VkDescriptorSet  ImGui_ImplVulkan_AddTexture(VkSampler sampler, VkImageView image_view, VkImageLayout image_layout);
IMGUI_IMPL_API void             ImGui_ImplVulkan_RemoveTexture(VkDescriptorSet descriptor_set);

// Optional: load Vulkan functions with a custom function loader
// This is only useful with IMGUI_IMPL_VULKAN_NO_PROTOTYPES / VK_NO_PROTOTYPES
IMGUI_IMPL_API bool             ImGui_ImplVulkan_LoadFunctions(PFN_vkVoidFunction(*loader_func)(const char* function_name, void* user_data), void* user_data = nullptr);

// [BETA] Selected render state data shared with callbacks.
// This is temporarily stored in GetPlatformIO().Renderer_RenderState during the ImGui_ImplVulkan_RenderDrawData() call.
// (Please open an issue if you feel you need access to more data)
struct ImGui_ImplVulkan_RenderState
{
    VkCommandBuffer     CommandBuffer;
    VkPipeline          Pipeline;
    VkPipelineLayout    PipelineLayout;
};

//-------------------------------------------------------------------------

struct ImGui_ImplVulkanH_Frame;
struct ImGui_ImplVulkanH_Window;

// Helpers
IMGUI_IMPL_API void                 ImGui_ImplVulkanH_CreateOrResizeWindow(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device, ImGui_ImplVulkanH_Window* wd, uint32_t queue_family, const VkAllocationCallbacks* allocator, int w, int h, uint32_t min_image_count);
IMGUI_IMPL_API void                 ImGui_ImplVulkanH_DestroyWindow(VkInstance instance, VkDevice device, ImGui_ImplVulkanH_Window* wd, const VkAllocationCallbacks* allocator);
IMGUI_IMPL_API VkSurfaceFormatKHR   ImGui_ImplVulkanH_SelectSurfaceFormat(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const VkFormat* request_formats, int request_formats_count, VkColorSpaceKHR request_color_space);
IMGUI_IMPL_API VkPresentModeKHR     ImGui_ImplVulkanH_SelectPresentMode(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const VkPresentModeKHR* request_modes, int request_modes_count);
IMGUI_IMPL_API VkPhysicalDevice     ImGui_ImplVulkanH_SelectPhysicalDevice(VkInstance instance);
IMGUI_IMPL_API uint32_t             ImGui_ImplVulkanH_SelectQueueFamilyIndex(VkPhysicalDevice physical_device);
IMGUI_IMPL_API int                  ImGui_ImplVulkanH_GetMinImageCountFromPresentMode(VkPresentModeKHR present_mode);

// Helper structure to hold the data needed by one rendering frame
// (Used by example's main.cpp. Used by multi-viewport features. Probably NOT used by your own engine/app.)
// [Please zero-clear before use!]
struct ImGui_ImplVulkanH_Frame
{
    VkCommandPool       CommandPool;
    VkCommandBuffer     CommandBuffer;
    VkFence             Fence;
    VkImage             Backbuffer;
    VkImageView         BackbufferView;
    VkFramebuffer       Framebuffer;
};

struct ImGui_ImplVulkanH_FrameSemaphores
{
    VkSemaphore         ImageAcquiredSemaphore;
    VkSemaphore         RenderCompleteSemaphore;
};

// Helper structure to hold the data needed by one rendering context into one OS window
// (Used by example's main.cpp. Used by multi-viewport features. Probably NOT used by your own engine/app.)
struct ImGui_ImplVulkanH_Window
{
    int                 Width;
    int                 Height;
    VkSwapchainKHR      Swapchain;
    VkSurfaceKHR        Surface;
    VkSurfaceFormatKHR  SurfaceFormat;
    VkPresentModeKHR    PresentMode;
    VkRenderPass        RenderPass;
    bool                UseDynamicRendering;
    bool                ClearEnable;
    VkClearValue        ClearValue;
    uint32_t            FrameIndex;             // Current frame being rendered to (0 <= FrameIndex < FrameInFlightCount)
    uint32_t            ImageCount;             // Number of simultaneous in-flight frames (returned by vkGetSwapchainImagesKHR, usually derived from min_image_count)
    uint32_t            SemaphoreCount;         // Number of simultaneous in-flight frames + 1, to be able to use it in vkAcquireNextImageKHR
    uint32_t            SemaphoreIndex;         // Current set of swapchain wait semaphores we're using (needs to be distinct from per frame data)
    ImVector<ImGui_ImplVulkanH_Frame>           Frames;
    ImVector<ImGui_ImplVulkanH_FrameSemaphores> FrameSemaphores;

    ImGui_ImplVulkanH_Window()
    {
        memset((void*)this, 0, sizeof(*this));
        PresentMode = (VkPresentModeKHR)~0;     // Ensure we get an error if user doesn't set this.
        ClearEnable = true;
    }
};

// ============================ Hydragon's Vulkan Backend ============================

namespace hd {

/**
 * @brief Defines the required Vulkan features, extensions, and validation layers needed
 * by the application. Used during device selection and initialization to ensure
 * the selected device meets all application requirements.
 */
struct DeviceRequirements
{
    std::vector<const char*> requiredExtensions;
    std::vector<const char*> validationLayers;
    VkPhysicalDeviceFeatures requiredFeatures;
};

/**
 * @brief Represents the indices of queue families required for graphics and presentation operations.
 * Used to determine if the selected physical device supports the required queue families.
 */
struct QueueFamilyIndices
{       
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

/**
 * @brief Contains details about the swap chain support provided by a physical device.
 * Used to determine if the selected physical device supports the required swap chain features.
 */
struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

/**
 * @brief Initialization information for the Vulkan backend.
 * Contains all the necessary handles and settings required for initializing the Vulkan backend.
 */
struct InitInfo
{
    VkInstance                      Instance;
    VkPhysicalDevice                PhysicalDevice;
    VkDevice                        Device;
    uint32_t                        QueueFamily;
    VkQueue                         Queue;
    VkDescriptorPool                DescriptorPool;               // See requirements in note above; ignored if using DescriptorPoolSize > 0
    VkRenderPass                    RenderPass;                   // Ignored if using dynamic rendering
    uint32_t                        MinImageCount;                // >= 2
    uint32_t                        ImageCount;                   // >= MinImageCount
    VkSampleCountFlagBits           MSAASamples;                  // 0 defaults to VK_SAMPLE_COUNT_1_BIT

    // (Optional)
    VkPipelineCache                 PipelineCache;
    uint32_t                        Subpass;

    // (Optional) Set to create internal descriptor pool instead of using DescriptorPool
    uint32_t                        DescriptorPoolSize;

    // (Optional) Dynamic Rendering
    // Need to explicitly enable VK_KHR_dynamic_rendering extension to use this, even for Vulkan 1.3.
    bool                            UseDynamicRendering;

    // Dynamic Rendering is used by default in Hydragon's Vulkan backend
    VkPipelineRenderingCreateInfoKHR PipelineRenderingCreateInfo;


    // (Optional) Allocation, Debugging
    const VkAllocationCallbacks*    Allocator;          // TODO: Replace with VMA allocator, below. Need to check the syntax, all commands needed.
    // VMA allocator instance
    VmaAllocator                    VmaAllocator;
    void                            (*CheckVkResultFn)(VkResult err);
    VkDeviceSize                    MinAllocationSize;      // Minimum allocation size. Set to 1024*1024 to satisfy zealous best practices validation layer and waste a little memory.
};

/**
 * @brief Manages a pool of command buffers for a specific queue family.
 * Provides efficient allocation and management of command buffers used for recording and submitting Vulkan commands.
 */
struct CommandBufferPool
{
    VkCommandPool pool;
    std::vector<VkCommandBuffer> buffers;
};

/**
 * @brief Represents a buffer resource and its associated allocation.
 * Used for managing memory and resources for Vulkan buffers.
 */
struct BufferResource 
{
    VkBuffer buffer;
    VmaAllocation allocation;
    VkDeviceSize size;
    VkBufferUsageFlags usage;
};

/**
 * @brief Represents a shader module and its associated stage and entry point.
 * Used for managing shader resources and their compilation.
 */
struct ShaderModule 
{
    VkShaderModule module;
    VkShaderStageFlagBits stage;
    std::string entryPoint;
};

/**
 * @brief Represents synchronization primitives used for managing command buffer execution and resource access.
 * Used for ensuring proper synchronization between command buffers and resources.
 */
struct SyncPrimitives 
{
    std::vector<VkSemaphore> frameAvailable;
    std::vector<VkSemaphore> renderFinished;
    std::vector<VkFence> inFlightFences;
};

/**
 * @brief Represents a descriptor set layout and its associated bindings.
 * Used for creating and managing descriptor sets for shader resources.
 */
struct DescriptorSetLayout 
{        
    VkDescriptorSetLayout layout;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    uint32_t setNumber;
};

/**
 * @brief Stores the state required for rendering ImGui draw data.
 * Contains the command buffer and pipeline used for rendering.
 */
struct RenderState
{
    VkCommandBuffer     CommandBuffer;
    VkPipeline          Pipeline;
    VkPipelineLayout    PipelineLayout;
};

/**
 * @brief Contains information about the pipeline state.
 * Used for creating and managing graphics pipelines.
 */
struct PipelineStateInfo 
{
    VkPipelineLayout layout;
    VkPipelineCache cache;
    VkPipelineCreateFlags flags;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
};

/**
 * @brief Represents a single frame of the application.
 * Contains the command buffer, fence, backbuffer, backbuffer view, and framebuffer for rendering.
 */
struct Frame
{
    VkCommandPool       CommandPool;
    VkCommandBuffer     CommandBuffer;
    VkFence             Fence;
    VkImage             Backbuffer;
    VkImageView         BackbufferView;
    VkFramebuffer       Framebuffer;
};

/**
 * @brief Represents the semaphores used for synchronization between frames.
 * Contains the image acquired semaphore and the render complete semaphore.
 */
struct FrameSemaphores
{
    VkSemaphore         ImageAcquiredSemaphore;
    VkSemaphore         RenderCompleteSemaphore;
};

/**
 * @brief Represents a window and its associated Vulkan resources.
 * Contains the swapchain, surface, surface format, present mode, render pass, and frame data.
 */
struct Window
{
    int                         Width;
    int                         Height;
    VkSwapchainKHR              Swapchain;
    VkSurfaceKHR                Surface;
    VkSurfaceFormatKHR          SurfaceFormat;
    VkPresentModeKHR            PresentMode;
    VkRenderPass                RenderPass;
    bool                        UseDynamicRendering;
    bool                        ClearEnable;
    VkClearValue                ClearValue;
    uint32_t                    FrameIndex;             // Current frame being rendered to (0 <= FrameIndex < FrameInFlightCount)
    uint32_t                    ImageCount;             // Number of simultaneous in-flight frames (returned by vkGetSwapchainImagesKHR, usually derived from min_image_count)
    uint32_t                    SemaphoreCount;         // Number of simultaneous in-flight frames + 1, to be able to use it in vkAcquireNextImageKHR
    uint32_t                    SemaphoreIndex;         // Current set of swapchain wait semaphores we're using (needs to be distinct from per frame data)
    ImVector<Frame>             Frames;
    ImVector<FrameSemaphores>   FrameSemaphores;

    Window()
    {
        memset((void*)this, 0, sizeof(*this));
        PresentMode = (VkPresentModeKHR)~0;     // Ensure we get an error if user doesn't set this.
        ClearEnable = true;
    }
};
 
/**
 * @brief The main Vulkan backend class.
 * Provides the core functionality for rendering ImGui draw data and the engine's graphics using Vulkan.
 */
class VulkanCore {           // Main Backend Class
public:
    // === Singleton & Lifecycle ===
    /**
     * @brief Get the singleton instance of the VulkanCore class.
     * @return The singleton instance.
     */
    static VulkanCore& getInstance() {
        static VulkanCore instance;
        return instance;
    }
    
    /**
     * @brief Initialize the Vulkan backend with the provided initialization information.
     * @param info The initialization information.
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize(const InitInfo& info) { }   // In imgui the signature is bool Init(ImGui_ImplVulkan_InitInfo* info)

    /**
     * @brief Shutdown the Vulkan backend.
     */
    void shutdown() { }

    /**
     * @brief Check if the Vulkan backend is initialized.
     * @return True if initialized, false otherwise.
     */
    bool isInitialized() const noexcept { return initialized; }

    // === Error handling ===
    /**
     * @brief Get the last error encountered by the Vulkan backend.
     * @return The last error encountered.
     */
    VkResult getLastError() const noexcept { return lastError; }

    // === Device & Queue management ===
    /**
     * @brief Select the physical device for the Vulkan backend.
     * @param instance The Vulkan instance.
     * @return The selected physical device.
     */
    VkPhysicalDevice selectPhysicalDevice(VkInstance instance) {}

    /**
     * @brief Get the Vulkan device handle.
     * @return The Vulkan device handle.
     */
    VkDevice getDevice() const noexcept { return device; }
    
    /**
     * @brief Select the queue family index for the Vulkan backend.
     * @param physical_device The physical device.
     * @return The selected queue family index.
     */
    uint32_t selectQueueFamilyIndex(VkPhysicalDevice physical_device) { }

    // === Command & Frame management ===
    /**
     * @brief Begin single-time command buffer recording.
     * @return The command buffer for recording.
     */
    VkCommandBuffer beginSingleTimeCommands() { }

    /**
     * @brief End single-time command buffer recording and submit it.
     * @param command_buffer The command buffer to end and submit.
     */
    void endSingleTimeCommands(VkCommandBuffer command_buffer) { }
    
    /**
     * @brief Begin a new frame for rendering.
     */
    void beginFrame() { }                         // In imgui the signature is void NewFrame()
    
    /**
     * @brief End the current frame and present the rendered content.
     */
    void endFrame() { }                           // Not present in imgui's example code
    
    /**
     * @brief Render the ImGui draw data using the provided command buffer and pipeline.
     * @param draw_data The ImGui draw data.
     * @param command_buffer The command buffer to use for rendering.
     * @param pipeline The pipeline to use for rendering.
     */
    void renderDrawData(ImDrawData* draw_data, VkCommandBuffer command_buffer, VkPipeline pipeline = VK_NULL_HANDLE) {}
    void createBuffer(VkBufferUsageFlags usage, VkDeviceSize size, VkBuffer* buffer, VmaAllocation* allocation) { }
    void destroyBuffer(VkBuffer buffer, VmaAllocation allocation) { }

    // === Resource management - Textures, Fonts, Shaders, etc ===
    /**
     * @brief Add a texture to the Vulkan backend.
     * @param sampler The sampler to use for the texture.
     * @param image_view The image view of the texture.
     * @param image_layout The image layout of the texture.
     * @return The descriptor set representing the added texture.
     */
    VkDescriptorSet addTexture(VkSampler sampler, VkImageView image_view, VkImageLayout image_layout) {}
    
    /**
     * @brief Remove a texture from the Vulkan backend.
     * @param descriptor_set The descriptor set representing the texture to remove.
     */
    void removeTexture(VkDescriptorSet descriptor_set) { }
    
    /**
     * @brief Create the font texture for ImGui.
     * @return True if font texture creation is successful, false otherwise.
     */
    bool createFontsTexture() { }   // Should we handle it here or in ResourceManager?
    
    /**
     * @brief Destroy the font texture for ImGui.
     */
    void destroyFontsTexture() { }

    // === Window and surface management ===
    /**
     * @brief Create or resize a window and its associated Vulkan resources.
     * @param instance The Vulkan instance.
     * @param physical_device The physical device.
     * @param device The Vulkan device.
     * @param wd The window data structure to create or resize.
     * @param queue_family The queue family index.
     * @param allocator The allocation callbacks.
     * @param w The width of the window.
     * @param h The height of the window.
     * @param min_image_count The minimum number of images in the swapchain.
     */
    void createOrResizeWindow(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device, 
        Window* wd, uint32_t queue_family, const VkAllocationCallbacks* allocator,
        int w, int h, uint32_t min_image_count) {}

    /**
     * @brief Destroy a window and its associated Vulkan resources.
     * @param instance The Vulkan instance.
     * @param device The Vulkan device.
     * @param wd The window data structure to destroy.
     * @param allocator The allocation callbacks.
     */
        void destroyWindow(VkInstance instance, VkDevice device, Window* wd,
    const VkAllocationCallbacks* allocator) {}

    /**
     * @brief Select a surface format for the given physical device and surface.
     * @param physical_device The physical device.
     * @param surface The surface.
     * @param request_formats The requested formats.
     * @param request_formats_count The number of requested formats.
     * @param request_color_space The requested color space.
     * @return The selected surface format.
     */
    VkSurfaceFormatKHR selectSurfaceFormat(VkPhysicalDevice physical_device, VkSurfaceKHR surface,
        const VkFormat* request_formats, int request_formats_count,
        VkColorSpaceKHR request_color_space) {}

    /**
     * @brief Select a present mode for the given physical device and surface.
     * @param physical_device The physical device.
     * @param surface The surface.
     * @param request_modes The requested present modes.
     * @param request_modes_count The number of requested present modes.
     * @return The selected present mode.
     */
        VkPresentModeKHR selectPresentMode(VkPhysicalDevice physical_device, VkSurfaceKHR surface,
        const VkPresentModeKHR* request_modes, int request_modes_count) {}

    // === Configuration ===
    /**
     * @brief Set the minimum image count for the swapchain.
     * @param min_image_count The minimum image count.
     */
    void setMinImageCount(uint32_t min_image_count) { }
    
    /**
     * @brief Enable or disable dynamic rendering.
     * @param enable True to enable dynamic rendering, false to disable.
     */
    void setDynamicRendering(bool enable) noexcept { }
    
    /**
     * @brief Load Vulkan functions using a custom function loader.
     * @param loader_func The function loader.
     * @param user_data The user data to pass to the function loader.
     * @return True if function loading is successful, false otherwise.
     */
    bool loadFunctions(PFN_vkVoidFunction(*loader_func)(const char* function_name, void* user_data), void* user_data = nullptr) {}

    /**
     * @brief Get the minimum image count from the given present mode.
     * @param present_mode The present mode.
     * @return The minimum image count.
     */
    int getMinImageCountFromPresentMode(VkPresentModeKHR present_mode) { }

    ~VulkanCore();

private:
    // 1. Core Data
    VkDevice device = VK_NULL_HANDLE;
    VmaAllocator allocator = VK_NULL_HANDLE;
    bool initialized = false;
    VkResult lastError = VK_SUCCESS;

    // Resource Management
    std::vector<BufferResource> managedBuffers;
    std::mutex renderMutex;                     // Mutex for thread-safe rendering operations

    // Private Methods
    bool validateDevice() const noexcept;
    void setLastError(VkResult error) noexcept { lastError = error; }
    void createCommandPools();
    void setupDescriptorPool();

    // 3. Singleton Implementation
    VulkanCore() noexcept;
    VulkanCore(const VulkanCore&) = delete;
    VulkanCore& operator=(const VulkanCore&) = delete;
    VulkanCore(VulkanCore&&) = delete;
    VulkanCore& operator=(VulkanCore&&) = delete;  
};

} // namespace hd
