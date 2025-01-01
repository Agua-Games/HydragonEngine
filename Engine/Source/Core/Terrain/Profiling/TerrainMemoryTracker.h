/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory tracking for terrain systems
 */

#pragma once
#include "Core/Memory/Profiling/MemoryProfiler.h"
#include "Core/Common/Singleton.h"

namespace Hydragon {
namespace Terrain {

class TerrainMemoryTracker : public Singleton<TerrainMemoryTracker> {
public:
    struct MemoryStats {
        size_t totalMeshletMemory = 0;
        size_t totalVertexMemory = 0;
        size_t totalIndexMemory = 0;
        size_t virtualTextureMemory = 0;
        size_t streamingBufferMemory = 0;
        uint32_t activeMeshletCount = 0;
        uint32_t streamedMeshletCount = 0;
        float fragmentationRatio = 0.0f;
    };

    void trackMeshletAllocation(size_t size) {
        m_Stats.totalMeshletMemory += size;
        m_MemoryProfiler.trackAllocation("TerrainMeshlet", size);
    }

    void trackMeshletDeallocation(size_t size) {
        m_Stats.totalMeshletMemory -= size;
        m_MemoryProfiler.trackDeallocation("TerrainMeshlet", size);
    }

    const MemoryStats& getStats() const { return m_Stats; }
    void resetStats() { m_Stats = MemoryStats(); }

private:
    MemoryStats m_Stats;
    Memory::MemoryProfiler m_MemoryProfiler;
};

}} // namespace Hydragon::Terrain 