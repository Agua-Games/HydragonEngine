/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#pragma once
#include "Core/Memory/GPU/GPUMemoryManager.h"
#include "Core/Memory/GPU/GPUMemoryDefrag.h"

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

    void update();
    void optimizeMemoryLayout();
    void predictStreamingNeeds(const Camera& camera);
    void defragmentIfNeeded();

    // Memory management
    void registerStreamingAllocation(size_t size, uint32_t regionId);
    void unregisterStreamingAllocation(uint32_t regionId);
    
    // Statistics
    float getFragmentationRatio() const;
    size_t getAvailableMemory() const;
    const std::vector<MemoryBlock>& getMemoryBlocks() const;

private:
    struct StreamingAllocation {
        size_t size;
        uint32_t regionId;
        bool isPredicted;
    };

    GPUMemoryManager& m_MemoryManager;
    GPUMemoryDefrag m_Defragmenter;
    OptimizationConfig m_Config;
    
    std::vector<StreamingAllocation> m_Allocations;
    uint32_t m_FramesSinceDefrag = 0;

    void compactMemory();
    void reallocateRegion(uint32_t regionId);
    bool shouldDefragment() const;
};

}} // namespace Hydragon::Terrain 