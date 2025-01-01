/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Virtual texturing system for terrain materials
 */

#pragma once
#include "Core/Graphics/RenderAPI/IRenderDevice.h"
#include "Core/Memory/Virtualization/VirtualMemoryManager.h"

namespace Hydragon {
namespace Terrain {

class TerrainVirtualTexturing {
public:
    struct VirtualTextureConfig {
        uint32_t pageSize = 128;          // Size of each texture page
        uint32_t feedbackBufferScale = 4; // Resolution scale for feedback
        uint32_t maxResidentPages = 2048; // Maximum resident pages in memory
        bool enableCompression = true;    // Use texture compression
        uint32_t mipLevels = 8;          // Number of mip levels to maintain
    };

    TerrainVirtualTexturing(IRenderDevice* device, const VirtualTextureConfig& config);
    ~TerrainVirtualTexturing();

    // Core virtual texturing operations
    void updateFeedback(CommandBuffer* cmd);
    void processPageRequests();
    void streamPages();

    // Memory management
    void evictPages(uint32_t count);
    size_t getResidentMemory() const;
    
    // Debug visualization
    void drawPageTable(CommandBuffer* cmd);
    void drawFeedbackBuffer(CommandBuffer* cmd);

private:
    struct PageTableEntry {
        uint32_t pageId;
        uint16_t mipLevel;
        uint16_t flags;
        float priority;
    };

    struct PageRequest {
        uint32_t pageId;
        uint16_t mipLevel;
        float priority;
    };

    IRenderDevice* m_Device;
    VirtualTextureConfig m_Config;
    Memory::VirtualMemoryManager m_VirtualMemory;

    // Page management
    std::vector<PageTableEntry> m_PageTable;
    std::queue<PageRequest> m_PendingRequests;
    
    // GPU resources
    TextureHandle m_FeedbackBuffer;
    TextureHandle m_PageTableTexture;
    BufferHandle m_PageRequestBuffer;

    void initializeResources();
    void updatePageTable();
    bool loadPage(const PageRequest& request);
};

}} // namespace Hydragon::Terrain 