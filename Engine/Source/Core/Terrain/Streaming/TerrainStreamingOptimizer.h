/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Terrain streaming memory optimization
 */

#pragma once

#include "Core/Camera/Camera.h"
#include "Core/Memory/GPU/GPUMemoryManager.h"
#include "Core/Memory/MemoryBlock.h"
#include "TerrainStreamingErrors.h"

namespace Hydragon {
namespace Terrain {

class TerrainStreamingOptimizer {
public:
    struct OptimizationConfig {
        bool enableDefragmentation = true;
        bool enablePrediction = true;
        float predictionThreshold = 0.75f;
        uint32_t defragInterval = 60;  // frames
    };

    TerrainStreamingOptimizer(GPUMemoryManager& memoryManager);
    ~TerrainStreamingOptimizer() = default;

    // Core operations with error handling
    Result<void> Update();
    Result<void> OptimizeMemoryLayout();
    Result<void> PredictStreamingNeeds(const Camera& camera);
    Result<void> DefragmentIfNeeded();

    // Memory management with error handling
    Result<void> RegisterStreamingAllocation(size_t size, uint32_t regionId);
    Result<void> UnregisterStreamingAllocation(uint32_t regionId);
    
    // Statistics
    float GetFragmentationRatio() const;
    size_t GetAvailableMemory() const;
    const std::vector<MemoryBlock>& GetMemoryBlocks() const;

private:
    struct StreamingAllocation {
        size_t size;
        uint32_t regionId;
        bool isPredicted;
    };

    GPUMemoryManager& m_memoryManager;
    GPUMemoryDefrag m_defragmenter;
    OptimizationConfig m_config;
    
    std::vector<StreamingAllocation> m_allocations;
    uint32_t m_framesSinceDefrag = 0;

    Result<void> CompactMemory();
    Result<void> ReallocateRegion(uint32_t regionId);
    bool ShouldDefragment() const;

    // Prevent copying
    TerrainStreamingOptimizer(const TerrainStreamingOptimizer&) = delete;
    TerrainStreamingOptimizer& operator=(const TerrainStreamingOptimizer&) = delete;
};

}} // namespace Hydragon::Terrain 