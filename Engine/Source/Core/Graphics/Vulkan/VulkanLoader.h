/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Hydragon's Vulkan Loader
 */
#pragma once
#include <vulkan/vulkan.h>

#if 0
namespace hd {

class VulkanLoader {                // Not present in ImGui's Vulkan backend. Remove this comment later.
    public:
        static bool LoadVulkanLibrary();
        static void FreeVulkanLibrary();
        static bool LoadExportedEntryPoints();
        static bool LoadGlobalLevelEntryPoints();
        static bool LoadInstanceLevelEntryPoints(VkInstance instance);
        static bool LoadDeviceLevelEntryPoints(VkDevice device);
    private:
        static void* vulkanLibrary;
        // Function pointers for core Vulkan commands
        static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        static PFN_vkCreateInstance vkCreateInstance;
        // etc...
    };

} // namespace hd

#endif