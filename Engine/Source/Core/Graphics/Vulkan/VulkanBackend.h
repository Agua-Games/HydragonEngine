#pragma once

#include <vulkan/vulkan.hpp>
#include <volk.h>
#include <vk_mem_alloc.h>
#include <vector>
#include <memory>
#include <optional>
#include <array>

namespace hd {
namespace Graphics {

// Forward declarations
class VulkanDevice;
class VulkanSwapchain;
class VulkanCommandPool;
class VulkanDescriptorPool;
class VulkanRenderPass;
class VulkanPipeline;

struct VulkanBackendCreateInfo {
    bool enableValidationLayers = true;
    std::vector<const char*> requiredExtensions;
    std::vector<const char*> requiredValidationLayers;
    uint32_t maxFramesInFlight = 2;
};

class VulkanBackend {
public:
    VulkanBackend() = default;
    ~VulkanBackend();

    // Prevent copying
    VulkanBackend(const VulkanBackend&) = delete;
    VulkanBackend& operator=(const VulkanBackend&) = delete;

    bool initialize(const VulkanBackendCreateInfo& createInfo);
    void shutdown();

    // ImGui integration
    bool initializeImGui();
    void shutdownImGui();
    void beginImGuiFrame();
    void endImGuiFrame();

    // Core Vulkan getters
    VkInstance getInstance() const { return m_instance; }
    VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }
    VkDevice getDevice() const { return m_device; }
    VmaAllocator getAllocator() const { return m_allocator; }

    // Queue family indices
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;
        std::optional<uint32_t> presentFamily;
        
        bool isComplete() const {
            return graphicsFamily.has_value() && 
                   computeFamily.has_value() && 
                   transferFamily.has_value() && 
                   presentFamily.has_value();
        }
    };

private:
    bool createInstance(const VulkanBackendCreateInfo& createInfo);
    bool setupDebugMessenger();
    bool pickPhysicalDevice();
    bool createLogicalDevice();
    bool createAllocator();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool checkValidationLayerSupport();

private:
    // Core Vulkan objects
    VkInstance m_instance{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT m_debugMessenger{VK_NULL_HANDLE};
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VmaAllocator m_allocator{VK_NULL_HANDLE};

    // Queue handles
    VkQueue m_graphicsQueue{VK_NULL_HANDLE};
    VkQueue m_computeQueue{VK_NULL_HANDLE};
    VkQueue m_transferQueue{VK_NULL_HANDLE};
    VkQueue m_presentQueue{VK_NULL_HANDLE};

    // ImGui specific members
    VkDescriptorPool m_imguiPool{VK_NULL_HANDLE};

    // Device features and extensions
    std::vector<const char*> m_deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
        VK_KHR_MAINTENANCE2_EXTENSION_NAME,
        VK_EXT_MESH_SHADER_EXTENSION_NAME,
        VK_KHR_SPIRV_1_4_EXTENSION_NAME,
        VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME
    };

    const std::vector<const char*> m_validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef NDEBUG
        const bool m_enableValidationLayers = false;
    #else
        const bool m_enableValidationLayers = true;
    #endif
};

} // namespace Graphics
} // namespace hd
