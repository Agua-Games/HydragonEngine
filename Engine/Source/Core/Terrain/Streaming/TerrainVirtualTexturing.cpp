/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Virtual texturing system implementation
 */

#include "TerrainVirtualTexturing.h"
#include "Core/Debug/ImGui/ImGuiRenderer.h"
#include "Core/Graphics/RenderAPI/CommandBuffer.h"
#include "Core/Memory/GPU/GPUMemoryTypes.h"
#include "Core/Profiling/Profiler.h"
#include "TerrainStreamingSystem.h"

namespace Hydragon {
namespace Terrain {

TerrainVirtualTexturing::TerrainVirtualTexturing(IRenderDevice* device, const VirtualTextureConfig& config)
    : m_Device(device)
    , m_Config(config) {
    
    HD_PROFILE_SCOPE("TerrainVirtualTexturing::Initialize");
    
    // Initialize virtual memory for texture pages
    Memory::VirtualMemoryManager::Config vmConfig;
    vmConfig.pageSize = config.pageSize * config.pageSize * 16; // RGBA format
    vmConfig.maxVirtualMemory = vmConfig.pageSize * config.maxResidentPages;
    
    m_VirtualMemory.initialize(vmConfig);
    initializeResources();
}

void TerrainVirtualTexturing::initializeResources() {
    // Create feedback buffer
    TextureDesc feedbackDesc;
    feedbackDesc.width = m_Device->getScreenWidth() / m_Config.feedbackBufferScale;
    feedbackDesc.height = m_Device->getScreenHeight() / m_Config.feedbackBufferScale;
    feedbackDesc.format = TextureFormat::R16G16_UINT; // Page and mip requests
    feedbackDesc.usage = TextureUsage::UAV | TextureUsage::ShaderResource;
    
    m_FeedbackBuffer = m_Device->createTexture(feedbackDesc);

    // Create page table texture
    TextureDesc pageTableDesc;
    pageTableDesc.width = m_Config.pageSize;
    pageTableDesc.height = m_Config.maxResidentPages;
    pageTableDesc.format = TextureFormat::R32G32_UINT; // Page info
    pageTableDesc.usage = TextureUsage::UAV | TextureUsage::ShaderResource;
    
    m_PageTableTexture = m_Device->createTexture(pageTableDesc);
}

void TerrainVirtualTexturing::updateFeedback(CommandBuffer* cmd) {
    HD_PROFILE_SCOPE("TerrainVirtualTexturing::UpdateFeedback");

    // Bind feedback compute shader
    cmd->bindComputeShader(m_Device->getShader("TerrainVT_Feedback"));
    
    // Read back feedback buffer and generate page requests
    auto* feedbackData = cmd->mapBuffer(m_FeedbackBuffer);
    if (feedbackData) {
        for (uint32_t y = 0; y < m_FeedbackBuffer->getHeight(); ++y) {
            for (uint32_t x = 0; x < m_FeedbackBuffer->getWidth(); ++x) {
                auto request = processFeedbackPixel(feedbackData, x, y);
                if (request.pageId != INVALID_PAGE) {
                    m_PendingRequests.push(request);
                }
            }
        }
        cmd->unmapBuffer(m_FeedbackBuffer);
    }
}

void TerrainVirtualTexturing::processPageRequests() {
    HD_PROFILE_SCOPE("TerrainVirtualTexturing::ProcessPageRequests");

    // Process a limited number of requests per frame
    constexpr uint32_t MAX_REQUESTS_PER_FRAME = 8;
    uint32_t processed = 0;

    while (!m_PendingRequests.empty() && processed < MAX_REQUESTS_PER_FRAME) {
        auto request = m_PendingRequests.front();
        m_PendingRequests.pop();

        if (loadPage(request)) {
            processed++;
            updatePageTable();
        }
    }
}

bool TerrainVirtualTexturing::loadPage(const PageRequest& request) {
    // Check if we need to evict pages first
    if (getResidentMemory() >= m_Config.maxResidentPages * m_Config.pageSize) {
        evictPages(8); // Evict batch of pages
    }

    // Allocate virtual memory for the page
    size_t pageSize = m_Config.pageSize * m_Config.pageSize * 16;
    void* pageMemory = m_VirtualMemory.allocate(pageSize);
    
    if (!pageMemory) {
        return false;
    }

    // Load page data (async operation)
    return streamPageData(request.pageId, request.mipLevel, pageMemory);
}

void TerrainVirtualTexturing::evictPages(uint32_t count) {
    HD_PROFILE_SCOPE("TerrainVirtualTexturing::EvictPages");

    // Sort pages by priority and last access time
    std::vector<PageTableEntry*> candidates;
    for (auto& entry : m_PageTable) {
        if (entry.flags & PAGE_FLAG_RESIDENT) {
            candidates.push_back(&entry);
        }
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const PageTableEntry* a, const PageTableEntry* b) {
            return a->priority < b->priority;
        });

    // Evict lowest priority pages
    for (uint32_t i = 0; i < count && i < candidates.size(); ++i) {
        auto* entry = candidates[i];
        m_VirtualMemory.deallocate(getPageAddress(entry->pageId));
        entry->flags &= ~PAGE_FLAG_RESIDENT;
        updatePageTableEntry(*entry);
    }
}

void TerrainVirtualTexturing::updatePageTable() {
    HD_PROFILE_SCOPE("TerrainVirtualTexturing::UpdatePageTable");

    // Update GPU page table texture
    auto* tableData = m_Device->mapTexture(m_PageTableTexture);
    if (tableData) {
        for (const auto& entry : m_PageTable) {
            uint32_t offset = entry.pageId * sizeof(PageTableEntry);
            memcpy(tableData + offset, &entry, sizeof(PageTableEntry));
        }
        m_Device->unmapTexture(m_PageTableTexture);
    }

    // Notify shader system of page table updates
    m_Device->setTextureDebugName(m_PageTableTexture, "TerrainVT_PageTable");
}

bool TerrainVirtualTexturing::streamPageData(uint32_t pageId, uint16_t mipLevel, void* memory) {
    HD_PROFILE_SCOPE("TerrainVirtualTexturing::StreamPageData");

    // Calculate page coordinates
    uint32_t pageX = pageId % m_Config.pageSize;
    uint32_t pageY = pageId / m_Config.pageSize;

    // Request data from streaming system
    StreamRequest request;
    request.x = pageX * m_Config.pageSize;
    request.y = pageY * m_Config.pageSize;
    request.width = m_Config.pageSize;
    request.height = m_Config.pageSize;
    request.mipLevel = mipLevel;
    request.destination = memory;

    // Queue async streaming request
    return m_StreamingSystem->queueRequest(request);
}

void TerrainVirtualTexturing::drawDebugUI() {
    if (ImGui::Begin("Virtual Texturing Debug")) {
        ImGui::Text("Resident Pages: %d/%d", getResidentPageCount(), m_Config.maxResidentPages);
        ImGui::Text("Memory Usage: %.2f MB", getResidentMemory() / (1024.0f * 1024.0f));
        ImGui::Text("Pending Requests: %d", m_PendingRequests.size());

        if (ImGui::CollapsingHeader("Page Table")) {
            drawPageTableDebug();
        }

        if (ImGui::CollapsingHeader("Feedback Buffer")) {
            drawFeedbackBufferDebug();
        }
    }
    ImGui::End();
}

}} // namespace Hydragon::Terrain 