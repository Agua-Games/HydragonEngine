/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Terrain meshlet system implementation
 */

#include "TerrainMeshletSystem.h"
#include "Core/Memory/Profiling/MemoryProfiler.h"
#include "Core/Debug/Visualization/TerrainDebugViz.h"
#include "Core/Graphics/Meshlet/MeshletBuilder.h"

namespace Hydragon {
namespace Terrain {

class TerrainMeshletSystem::Impl {
    struct MeshletCache {
        GPUMemoryPool m_meshletPool;
        ThreadSafeQueue<MeshletHandle> m_availableMeshlets;
        std::unordered_map<ChunkId, std::vector<MeshletHandle>> m_activeMeshlets;
    };

    MemoryProfiler m_memoryProfiler;
    MeshletCache m_cache;
    TerrainDebugVizPtr m_debugViz;
};

TerrainMeshletSystem::TerrainMeshletSystem(const TerrainMeshletConfig& config)
    : m_impl(std::make_unique<Impl>())
    , m_config(config) {
    
    // Initialize GPU memory pool for meshlets
    GPUMemoryDesc meshletPoolDesc{
        .size = config.meshletPoolSize,
        .flags = GPUMemoryFlags::VertexBuffer | GPUMemoryFlags::IndexBuffer,
        .name = "TerrainMeshletPool"
    };
    if (!m_impl->m_cache.m_meshletPool.Initialize(meshletPoolDesc)) {
        throw std::runtime_error("Failed to initialize meshlet pool");
    }

    // Setup debug visualization
    m_impl->m_debugViz = std::make_shared<TerrainDebugViz>();
}

void TerrainMeshletSystem::ProcessChunk(const TerrainChunkData& chunk) {
    PROFILE_SCOPE("TerrainMeshletSystem::ProcessChunk");
    
    // Clean up old meshlets for this chunk if they exist
    if (auto it = m_impl->m_cache.m_activeMeshlets.find(chunk.id); 
        it != m_impl->m_cache.m_activeMeshlets.end()) {
        ReleaseMeshlets(it->second);
        m_impl->m_cache.m_activeMeshlets.erase(it);
    }
    
    // Generate meshlets based on terrain complexity
    auto meshlets = GenerateMeshletsForChunk(chunk);

    // Update debug visualization if enabled
    if (m_debugSettings.visualizeMeshlets) {
        m_impl->m_debugViz->UpdateMeshletVisualization(meshlets);
    }

    // Upload to GPU memory
    UploadMeshlets(chunk.id, meshlets);

    // Track memory usage
    m_impl->m_memoryProfiler.TrackAllocation(
        "TerrainMeshlets",
        CalculateMeshletMemoryUsage(meshlets)
    );
}

std::vector<TerrainMeshlet> TerrainMeshletSystem::GenerateMeshletsForChunk(
    const TerrainChunkData& chunk) {
    
    MeshletBuilder builder;
    builder.SetTargetTrianglesPerMeshlet(m_config.targetTrianglesPerMeshlet);
    builder.SetLODLevel(DetermineLODLevel(chunk));

    // Generate base meshlets
    auto meshlets = builder.BuildMeshlets(chunk);

    // Apply LOD transitions
    ApplyLODTransitions(meshlets, chunk);

    return meshlets;
}

void TerrainMeshletSystem::UpdateDebugVisualization() {
    if (!m_debugSettings.enabled) return;

    auto& debugViz = m_impl->m_debugViz;

    // Update visualization modes
    if (m_debugSettings.showMeshletBounds) {
        debugViz->RenderMeshletBounds(m_impl->m_cache.m_activeMeshlets);
    }

    if (m_debugSettings.showVertexDensity) {
        debugViz->RenderVertexDensityHeatmap(m_impl->m_cache.m_activeMeshlets);
    }

    if (m_debugSettings.showLODTransitions) {
        debugViz->RenderLODTransitionRegions(m_impl->m_cache.m_activeMeshlets);
    }

    // Update memory stats
    debugViz->UpdateMemoryStats({
        .totalAllocated = m_impl->m_cache.m_meshletPool.GetAllocatedSize(),
        .totalAvailable = m_impl->m_cache.m_meshletPool.GetAvailableSize(),
        .meshletCount = m_impl->m_cache.m_activeMeshlets.size()
    });
}

bool TerrainMeshletSystem::ValidateChunkData(const TerrainChunkData& chunk) {
    if (chunk.heightfield.empty()) return false;
    if (chunk.resolution == 0) return false;
    if (chunk.bounds.IsEmpty()) return false;
    return true;
}

void TerrainMeshletSystem::CleanupUnusedMeshlets() {
    PROFILE_SCOPE("TerrainMeshletSystem::CleanupUnusedMeshlets");
    
    std::lock_guard<std::mutex> lock(m_impl->m_cacheMutex);
    
    const auto now = std::chrono::steady_clock::now();
    std::vector<ChunkId> chunksToRemove;
    
    for (const auto& [chunkId, meshlets] : m_impl->m_cache.m_activeMeshlets) {
        if (IsChunkUnused(chunkId, now)) {
            chunksToRemove.push_back(chunkId);
        }
    }
    
    for (auto chunkId : chunksToRemove) {
        ReleaseMeshlets(m_impl->m_cache.m_activeMeshlets[chunkId]);
        m_impl->m_cache.m_activeMeshlets.erase(chunkId);
    }
}

Result<std::vector<TerrainMeshlet>> TerrainMeshletSystem::GenerateMeshletsForChunk(
    const TerrainChunkData& chunk) {
    try {
        if (!ValidateChunkData(chunk)) {
            return Error("Invalid chunk data");
        }
        
        MeshletBuilder builder;
        builder.SetTargetTrianglesPerMeshlet(m_config.targetTrianglesPerMeshlet);
        builder.SetLODLevel(DetermineLODLevel(chunk));

        // Generate base meshlets
        auto meshlets = builder.BuildMeshlets(chunk);

        // Apply LOD transitions
        ApplyLODTransitions(meshlets, chunk);

        return meshlets;
    }
    catch (const std::exception& e) {
        return Error(fmt::format("Meshlet generation failed: {}", e.what()));
    }
}

} // namespace Hydragon::Terrain 