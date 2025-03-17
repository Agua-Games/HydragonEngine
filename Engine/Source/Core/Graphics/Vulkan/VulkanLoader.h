/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * @file VulkanLoader.h
 * @brief Hydragon's Vulkan Loader
 * 
 * ARCHITECTURAL NOTES:
 * 
 * TODO:
 * 
 */
#pragma once
#include <vulkan/vulkan.h>

#if 0
namespace hd {

class VulkanLoader {                // Not present in ImGui's Vulkan backend. Remove this comment later.
    public:
        static bool loadVulkanLibrary();
        static void freeVulkanLibrary();
        static bool loadExportedEntryPoints();
        static bool loadGlobalLevelEntryPoints();
        static bool loadInstanceLevelEntryPoints(VkInstance instance);
        static bool loadDeviceLevelEntryPoints(VkDevice device);
    private:
        static void* vulkanLibrary;
        // Function pointers for core Vulkan commands
        static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        static PFN_vkCreateInstance vkCreateInstance;
        // etc...
    };  // class VulkanLoader

} // namespace hd

#endif