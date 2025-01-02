/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#include "TerrainStreamingSystem.h"

#include "Core/Memory/Management/MemoryManager.h"
#include "Core/Memory/NUMA/NumaAllocator.h"
#include "Core/Memory/GPU/GPUMemoryPool.h"
#include "Core/Profiling/Profiler.h"
#include "Core/Tasks/TaskScheduler.h"
#include "Core/Time/TimeSystem.h"

namespace Hydragon {
namespace Terrain {

class TerrainStreamingSystem::Impl {
    struct StreamingCache {
        NUMAMemoryPool m_heightfieldPool;
        GPUMemoryPool m_texturePool;
        std::unordered_map<ChunkId, StreamingStatus> m_chunkStatus;
    };

    TaskScheduler m_streamingTasks;
    StreamingCache m_cache;
    MemoryProfiler m_memoryProfiler;
};

TerrainStreamingSystem::TerrainStreamingSystem(const StreamingConfig& config)
    : m_impl(std::make_unique<Impl>())
    , m_config(config) {
    
    // Initialize memory pools
    NUMAMemoryDesc heightfieldDesc{
        .size = config.heightfieldCacheSize,
        .nodePreference = NUMANodePreference::Local,
        .name = "TerrainHeightfieldCache"
    };
    m_impl->m_cache.m_heightfieldPool.Initialize(heightfieldDesc);

    // Initialize task system
    m_impl->m_streamingTasks.Initialize(config.maxStreamingThreads);
}

void TerrainStreamingSystem::Update(const ViewInfo& viewInfo) {
    PROFILE_SCOPE("TerrainStreamingSystem::Update");

    // Update streaming priorities
    UpdateStreamingPriorities(viewInfo);

    // Process streaming queue
    ProcessStreamingQueue();

    // Update memory tracking
    UpdateMemoryStats();
}

void TerrainStreamingSystem::UpdateStreamingPriorities(const ViewInfo& viewInfo) {
    struct ChunkPriority {
        ChunkId id;
        float priority;
    };
    
    std::vector<ChunkPriority> priorities;
    priorities.reserve(m_activeChunks.size());

    // Calculate priorities for all chunks
    for (const auto& chunk : m_activeChunks) {
        float distance = CalculateChunkDistance(chunk, viewInfo.position);
        float priority = CalculateStreamingPriority(chunk, distance);
        priorities.push_back({chunk.id, priority});
    }

    // Sort by priority
    std::sort(priorities.begin(), priorities.end(),
        [](const auto& a, const auto& b) {
            return a.priority > b.priority;
        });

    // Queue high priority chunks for streaming
    for (const auto& p : priorities) {
        if (p.priority > m_config.streamingThreshold) {
            QueueChunkForStreaming(p.id);
        }
    }
}

void TerrainStreamingSystem::ProcessStreamingQueue() {
    while (!m_streamingQueue.empty() && 
           m_impl->m_streamingTasks.HasAvailableThreads()) {
        
        auto request = m_streamingQueue.front();
        m_streamingQueue.pop();

        // Create streaming task
        auto task = [this, request]() {
            StreamChunkData(request);
        };

        m_impl->m_streamingTasks.ScheduleTask(std::move(task));
    }
}

void TerrainStreamingSystem::UpdateMemoryStats() {
    auto& profiler = m_impl->m_memoryProfiler;

    profiler.UpdateStats({
        .heightfieldMemory = m_impl->m_cache.m_heightfieldPool.GetUsedSize(),
        .textureMemory = m_impl->m_cache.m_texturePool.GetUsedSize(),
        .activeChunks = m_impl->m_cache.m_chunkStatus.size(),
        .pendingRequests = m_streamingQueue.size()
    });
}

} // namespace Hydragon::Terrain 