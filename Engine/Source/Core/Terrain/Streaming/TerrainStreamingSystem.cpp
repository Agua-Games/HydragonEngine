/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Terrain streaming system implementation
 */

#include "TerrainStreamingSystem.h"
#include "Core/Profiling/Profiler.h"
#include "Core/Memory/MemoryManager.h"

namespace Hydragon {
namespace Terrain {

TerrainStreamingSystem::TerrainStreamingSystem(const StreamingConfig& config)
    : m_Config(config) {
    
    HD_PROFILE_SCOPE("TerrainStreamingSystem::Initialize");

    // Initialize virtual memory for streaming
    Memory::VirtualMemoryManager::Config vmConfig;
    vmConfig.pageSize = 1024 * 1024;  // 1MB pages
    vmConfig.maxVirtualMemory = static_cast<size_t>(config.streamingBudgetMB) * 1024 * 1024;
    
    auto result = m_VirtualMemory.initialize(vmConfig);
    if (!result) {
        throw StreamingError::deviceNotSupported("Virtual memory initialization failed");
    }
}

Result<void> TerrainStreamingSystem::registerStreamingRegion(const StreamingRegion& region) {
    HD_PROFILE_SCOPE("TerrainStreamingSystem::RegisterRegion");

    if (!validateRegion(region)) {
        return Error("Invalid streaming region");
    }

    if (m_ActiveRegions.size() >= m_Config.maxActiveRegions) {
        cleanupUnusedRegions();
        if (m_ActiveRegions.size() >= m_Config.maxActiveRegions) {
            return StreamingError::outOfMemory("Maximum active regions reached");
        }
    }

    ActiveRegion activeRegion;
    activeRegion.region = region;
    activeRegion.loadProgress = 0.0f;
    activeRegion.isLoading = false;
    activeRegion.lastAccess = Time::now();

    m_ActiveRegions[region.id] = std::move(activeRegion);
    return Success;
}

void TerrainStreamingSystem::processStreamingQueue() {
    HD_PROFILE_SCOPE("TerrainStreamingSystem::ProcessQueue");

    while (!m_PendingRequests.empty()) {
        auto request = m_PendingRequests.top();
        m_PendingRequests.pop();

        auto it = m_ActiveRegions.find(request.regionId);
        if (it == m_ActiveRegions.end()) {
            continue;  // Region no longer exists
        }

        if (request.isLoad) {
            auto result = streamRegion(request.regionId);
            if (!result) {
                HD_LOG_ERROR("Failed to stream region {}: {}", 
                            request.regionId, result.error());
            }
        } else {
            auto result = unloadRegion(request.regionId);
            if (!result) {
                HD_LOG_ERROR("Failed to unload region {}: {}", 
                            request.regionId, result.error());
            }
        }

        updateMetrics(request);
    }
}

Result<void> TerrainStreamingSystem::streamRegion(uint32_t regionId) {
    HD_PROFILE_SCOPE("TerrainStreamingSystem::StreamRegion");

    auto it = m_ActiveRegions.find(regionId);
    if (it == m_ActiveRegions.end()) {
        return Error("Invalid region ID");
    }

    auto& region = it->second;
    if (region.isLoading) {
        return Error("Region is already loading");
    }

    // Allocate memory for the region
    auto result = allocateRegionMemory(regionId);
    if (!result) {
        return result;
    }

    region.isLoading = true;
    region.lastAccess = Time::now();

    // Start async loading
    auto loadResult = beginAsyncLoad(region);
    if (!loadResult) {
        region.isLoading = false;
        return loadResult;
    }

    return Success;
}

bool TerrainStreamingSystem::validateRegion(const StreamingRegion& region) const {
    if (region.bounds.isEmpty()) {
        HD_LOG_ERROR("Empty region bounds");
        return false;
    }

    if (region.priority < 0.0f || region.priority > 1.0f) {
        HD_LOG_ERROR("Invalid region priority");
        return false;
    }

    if (region.resolution == 0) {
        HD_LOG_ERROR("Invalid region resolution");
        return false;
    }

    return true;
}

void TerrainStreamingSystem::updateMetrics(const StreamingRequest& request) {
    auto& region = m_ActiveRegions[request.regionId];
    
    if (request.isLoad) {
        m_Metrics.totalBytesStreamed += region.region.getMemorySize();
        m_Metrics.activeRegionCount++;
    } else {
        m_Metrics.totalBytesUnloaded += region.region.getMemorySize();
        m_Metrics.activeRegionCount--;
    }

    m_Metrics.peakActiveRegions = 
        std::max(m_Metrics.peakActiveRegions, m_Metrics.activeRegionCount);
}

void TerrainStreamingSystem::cleanupUnusedRegions() {
    HD_PROFILE_SCOPE("TerrainStreamingSystem::CleanupRegions");

    const auto now = Time::now();
    std::vector<uint32_t> regionsToUnload;

    for (const auto& pair : m_ActiveRegions) {
        const auto& region = pair.second;
        auto timeSinceAccess = Time::duration(region.lastAccess, now);

        if (timeSinceAccess > Time::seconds(30) && !region.isLoading) {
            regionsToUnload.push_back(pair.first);
        }
    }

    for (auto regionId : regionsToUnload) {
        unloadRegion(regionId);
    }
}

}} // namespace Hydragon::Terrain 