/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Viewport manager for Vulkan.
 * Handles synchronization and presentation of multiple viewports.
 * - Synchronizes rendering across multiple viewports
 * - Manages presentation of viewports
 * - Handles swap chain recreation and viewport transitions
 * - Avoids flickering during viewport creation and transitions
 * 
 * TODO (some of these may be addressed not here but in VulkanBackend.h, .cpp):
 *  - Flesh out the class, structs, and functions
 *  - Add error handling and logging
 *  - Implement swap chain recreation and viewport transition handling
 *  - Add support for customizing the implementation of imgui_impl_vulkan.cpp
 *  - Further encapsulate Vulkan operations within the class
 *  - Add additional helper functions for resource management and performance optimization
 *  - Integrate Volk and ImGui helpers effectively
 *  - Add comments and documentation for better understanding and maintenance
 */
#pragma once
#include <vulkan/vulkan.h>

namespace hd {
class ViewportManager {
    struct SyncPoint {
        VkSemaphore renderComplete;
        VkFence inFlightFence;
    };
    
    void SynchronizeViewports() {
        // Ensure all viewports complete rendering before presentation
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        for (auto& viewport : m_viewports) {
            vkWaitForFences(device, 1, &viewport.syncPoint.inFlightFence, VK_TRUE, UINT64_MAX);
        }
    }
};

} // namespace hd