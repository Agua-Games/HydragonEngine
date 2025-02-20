/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Vulkan context class.
 * Wraps Vulkan handles and provides access to them.
 * 
 * TODO (some of these may be addressed not here but in VulkanBackend.h, .cpp):
 *  - Flesh out the class, structs, and functions
 */
#pragma once
#include <vulkan/vulkan.h>

namespace hd {

class VulkanContext {
public:
    // Wrap existing ImGui Vulkan globals/state
    void Initialize(const ImGui_ImplVulkan_InitInfo& init_info) {
        // Store existing Vulkan handles
        m_device = init_info.Device;
        m_physical_device = init_info.PhysicalDevice;
        // etc...
        
        // Still use ImGui's initialization for now
        ImGui_ImplVulkan_Init(&init_info);
    }
    
    // Getter methods to access Vulkan handles
    VkDevice GetDevice() const { return m_device; }
    VkPhysicalDevice GetPhysicalDevice() const { return m_physical_device; }
    
private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    // etc...
};

} // namespace hd