/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Terrain streaming system
 */

#pragma once
#include "Core/Memory/Virtualization/VirtualMemoryManager.h"
#include "TerrainStreamingErrors.h"
#include "TerrainTypes.h"

namespace Hydragon {
namespace Terrain {

class TerrainStreamingSystem {
public:
    struct StreamingConfig {
        uint32_t maxActiveRegions = 16;
        uint32_t streamingBudgetMB = 512;
        float streamingDistance = 1000.0f;
        bool enablePrioritization = true;
        uint32_t maxPendingRequests = 64;
    };

    TerrainStreamingSystem(const StreamingConfig& config);
    ~TerrainStreamingSystem();

    // Region management
    Result<void> registerStreamingRegion(const StreamingRegion& region);
    void unregisterStreamingRegion(uint32_t regionId);
    void updateRegionPriorities(const Vector3& viewerPosition);

    // Streaming operations
    Result<void> streamRegion(uint32_t regionId);
    Result<void> unloadRegion(uint32_t regionId);
    void processStreamingQueue();

    // Status and metrics
    bool isRegionLoaded(uint32_t regionId) const;
    float getRegionLoadProgress(uint32_t regionId) const;
    const StreamingMetrics& getMetrics() const { return m_Metrics; }

private:
    struct StreamingRequest {
        uint32_t regionId;
        float priority;
        bool isLoad;  // true = load, false = unload
    };

    struct ActiveRegion {
        StreamingRegion region;
        float loadProgress;
        bool isLoading;
        Time::Point lastAccess;
    };

    StreamingConfig m_Config;
    Memory::VirtualMemoryManager m_VirtualMemory;
    std::unordered_map<uint32_t, ActiveRegion> m_ActiveRegions;
    std::priority_queue<StreamingRequest> m_PendingRequests;
    StreamingMetrics m_Metrics;

    // Internal helpers
    bool validateRegion(const StreamingRegion& region) const;
    Result<void> allocateRegionMemory(uint32_t regionId);
    void updateMetrics(const StreamingRequest& request);
    void cleanupUnusedRegions();
};

}} // namespace Hydragon::Terrain 