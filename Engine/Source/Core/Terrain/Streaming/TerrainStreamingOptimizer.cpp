/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#include "TerrainStreamingOptimizer.h"
#include "Core/Profiling/Profiler.h"

namespace Hydragon {
namespace Terrain {

void TerrainStreamingOptimizer::update() {
    HD_PROFILE_SCOPE("TerrainStreamingOptimizer::Update");

    // Check if defragmentation is needed
    if (m_Config.enableDefragmentation) {
        defragmentIfNeeded();
    }

    // Update memory layout
    optimizeMemoryLayout();

    m_FramesSinceDefrag++;
}

void TerrainStreamingOptimizer::optimizeMemoryLayout() {
    HD_PROFILE_SCOPE("TerrainStreamingOptimizer::OptimizeLayout");

    // Sort allocations by access frequency
    std::sort(m_Allocations.begin(), m_Allocations.end(),
        [](const StreamingAllocation& a, const StreamingAllocation& b) {
            return a.isPredicted > b.isPredicted;
        });

    // Try to move frequently accessed data to faster memory
    for (auto& alloc : m_Allocations) {
        if (alloc.isPredicted) {
            if (auto fastMemory = m_MemoryManager.findFastMemory(alloc.size)) {
                reallocateRegion(alloc.regionId);
            }
        }
    }
}

void TerrainStreamingOptimizer::defragmentIfNeeded() {
    if (!shouldDefragment()) return;

    HD_PROFILE_SCOPE("TerrainStreamingOptimizer::Defragment");

    // Prepare defragmentation plan
    auto plan = m_Defragmenter.createDefragPlan(m_MemoryManager);
    
    if (plan.moveCount > 0) {
        // Execute defragmentation
        m_Defragmenter.executeDefragPlan(plan);
        m_FramesSinceDefrag = 0;
    }
}

void TerrainStreamingOptimizer::predictStreamingNeeds(const Camera& camera) {
    if (!m_Config.enablePrediction) return;

    HD_PROFILE_SCOPE("TerrainStreamingOptimizer::PredictNeeds");

    // Reset prediction flags
    for (auto& alloc : m_Allocations) {
        alloc.isPredicted = false;
    }

    // Predict based on camera movement
    Vector3 cameraVelocity = camera.getVelocity();
    float speed = cameraVelocity.length();

    if (speed > 1.0f) {
        // Project camera position
        Vector3 futurePos = camera.getPosition() + cameraVelocity * 2.0f;
        
        // Mark regions that will likely be needed
        for (auto& alloc : m_Allocations) {
            float distance = getRegionDistance(alloc.regionId, futurePos);
            if (distance < m_Config.predictionThreshold) {
                alloc.isPredicted = true;
            }
        }
    }
}

bool TerrainStreamingOptimizer::shouldDefragment() const {
    if (m_FramesSinceDefrag < m_Config.defragInterval) {
        return false;
    }

    float fragRatio = getFragmentationRatio();
    return fragRatio > 0.3f;  // 30% fragmentation threshold
}

float TerrainStreamingOptimizer::getFragmentationRatio() const {
    size_t totalSpace = m_MemoryManager.getTotalMemory();
    size_t usedSpace = m_MemoryManager.getUsedMemory();
    size_t largestBlock = m_MemoryManager.getLargestFreeBlock();

    return 1.0f - (static_cast<float>(largestBlock) / 
                   static_cast<float>(totalSpace - usedSpace));
}

}} // namespace Hydragon::Terrain 