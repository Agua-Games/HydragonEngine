/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TiledAtlasTexturing.h
 * @brief TiledAtlasTexturing is a system for managing tiled textures in a sparse atlas.
 * 
 * ARCHITECTURAL NOTES:
 * - Inspired by Id Software's Doom 3 Megatexture streaming system
 * - Uses Vulkan sparse binding/residency for (virtual) tiled textures
 * - Implements feedback using compute shaders and buffer readback
 * - Leverages VkSparseImageMemoryRequirements for page management
 * - Utilizes VkSparseImageMemoryBind for page updates
 * - Compatible with bindless texture approach
 * - Leverages VK_EXT_pageable_device_local_memory for residency priority
 * - Supports streaming bias and quality levels
 * 
 * TODO:
 * - Refactor the code to integrate the struct PageRequest into the TiledAtlasTexturingSystem class and the whole file architecture.
 * - Cleanup the whole content, after the first version of the VulkanCore/Renderer is working in its definitive architecture, refactoring
 * it carefully, in steps, always testing.
 */
#if 0
#pragma once
#include <vulkan/vulkan.h>
#include "Core/Graphics/TextureTypes.h"
#include "Core/Memory/PageAllocator.h"

namespace hd {

class TiledAtlasTexturingSystem {
    // === Structure Definitions ===
    public:
        // Vulkan-specific sparse texture configuration
        struct SparseTextureConfig {
            VkFormat format;
            VkImageUsageFlags usage;
            VkSparseImageFormatProperties sparseProperties;
            VkExtent3D pageSize;              // Typically 128x128 or 256x256
            uint32_t mipLevels;
            uint32_t arrayLayers;
        };

        // Page table structure
        struct PageTable {
            struct PageEntry {                 // Basic page data
                VkDeviceSize offset;          // Offset in sparse memory binding
                uint32_t mipLevel;
                uint32_t layer;
                bool resident;
            };
            
            // Maps virtual addresses to physical memory, for residency
            std::unordered_map<uint64_t, PageEntry> pageMap;
            VkBuffer pageTableBuffer;         // GPU-visible page table
            VkDeviceMemory pageTableMemory;
        };

        // Feedback system using compute shaders
        struct FeedbackSystem {
            struct FeedbackBuffer {
                VkBuffer buffer;
                VkDeviceMemory memory;
                VkDescriptorSet descriptorSet;
                uint32_t* mappedData;         // CPU-mapped feedback data
            };

            struct PageRequest {              // Feedback data for page requests
                uint32_t pageIndex;
                uint32_t mipLevel;
                uint32_t priority;
                float screenSpaceSize;
            };

            VkPipeline feedbackPipeline;      // Compute pipeline for page requests
            VkPipelineLayout pipelineLayout;
        };

        // Streaming system using sparse binding
        struct StreamingSystem {
            struct PendingBind {
                std::vector<VkSparseImageMemoryBind> imageBinds;
                std::vector<VkSparseMemoryBind> opaqueBinds;
                VkSparseImageOpaqueMemoryBindInfo opaqueInfo;
                VkSparseImageMemoryBindInfo imageInfo;
            };

            VkQueue sparseQueue;              // Queue for sparse binding operations
            VkSemaphore bindSemaphore;        // Sync for binding operations
            uint32_t maxConcurrentTransfers;
        };

        // Physical allocation management
        struct PhysicalAllocator {
            VkDeviceSize pageSize;
            VkDeviceSize totalBudget;
            VkDeviceMemory deviceMemory;      // Backing memory for resident pages
            
            struct MemoryPool {
                VkDeviceSize size;
                VkDeviceSize alignment;
                std::vector<VkDeviceSize> freeBlocks;
            };
        };

        // Runtime configuration
        struct RuntimeConfig {
            bool enableResidencyPriority;     // Use VK_EXT_pageable_device_local_memory
            bool useAsyncTransfers;           // Async transfer queue for loading
            bool enableTileCompression;       // Use compressed formats if available
            float loadBias;                   // Bias for mip level selection
        };

        // Add: Enhanced feedback metrics for smarter page management
        struct FeedbackMetrics {
            float screenSpaceCoverage;    // Visible area of page
            float viewingDistance;        // Distance from camera
            uint32_t framesSinceRequest;  // Request age
            bool isInFrustum;            // Frustum culling result
        };
        
        // Add: Extended page entry data for better prioritization
        struct ExtendedPageEntry : PageEntry {
            FeedbackMetrics metrics;
            uint32_t lastAccessFrame;     // For LRU tracking
            uint32_t accessCount;         // Usage frequency
            float priority;               // Computed priority
        };
        
        // Add: Memory budget controls
        struct MemoryBudget {
            VkDeviceSize totalLimit;      // Max VRAM usage
            float emergencyThreshold;     // Trigger emergency unload at %
            float targetUtilization;      // Desired VRAM usage %
            
            // Per-category budgets
            struct {
                float highPriority;       // Characters, important effects
                float mediumPriority;     // Environment, terrain
                float lowPriority;        // Background, far objects
            } categories;
        };
        
        // Add: Statistics for monitoring
        struct Statistics {
            uint32_t residentPages;
            uint32_t pendingRequests;
            float hitRate;
            VkDeviceSize currentMemoryUsage;
            uint32_t pageLoadThisFrame;
        };

        // === Allocation, Initialization, Loading ===
        // Initialize the system
        VkResult initialize(VkDevice device, const SparseTextureConfig& config);
        
        // Update page residency based on feedback
        void updateResidency(VkCommandBuffer cmd);
        
        // Bind/unbind pages
        VkResult bindPages(const std::vector<PageRequest>& requests);
        void evictPages(const std::vector<uint32_t>& pageIndices);
        
        // Add: New management functions
        void setMemoryBudget(const MemoryBudget& budget);
        Statistics getStatistics() const;
        void updatePagePriorities();
        void trimMemoryIfNeeded();
        
        // Add: Quality control
        void setQualityLevel(uint32_t level);  // 0-3, affects mip selection
        void setStreamingBias(float bias);     // -1 to 1, trading quality vs memory
        
    private:
        // Add: Internal helper functions
        float computePagePriority(const ExtendedPageEntry& page);
        void handleEmergencyTrim();
        bool shouldLoadPage(const FeedbackMetrics& metrics);
};

} // namespace hd

#endif

