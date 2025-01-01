/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory pressure analyzer for Hydragon
 */

#include "MemoryPressureAnalyzer.h"
#include <algorithm>

namespace Hydragon {
namespace Tools {

PressureMetrics MemoryPressureAnalyzer::calculateMetrics(const Memory::MemoryState& state) {
    PressureMetrics metrics{};
    
    // Calculate fragmentation ratio
    size_t totalFreeSpace = 0;
    size_t largestFree = 0;
    std::vector<size_t> freeBlocks;
    
    for (const auto& region : state.freeRegions) {
        totalFreeSpace += region.size;
        largestFree = std::max(largestFree, region.size);
        freeBlocks.push_back(region.size);
    }
    
    if (totalFreeSpace > 0) {
        metrics.fragmentationRatio = 1.0f - (float)largestFree / totalFreeSpace;
    }
    
    // Calculate page utilization
    size_t totalPages = state.totalMemory / state.pageSize;
    size_t usedPages = 0;
    
    for (const auto& block : state.blocks) {
        usedPages += (block.size + state.pageSize - 1) / state.pageSize;
    }
    
    metrics.pageUtilization = (float)usedPages / totalPages;
    
    // Calculate allocation rate and failure rate
    auto timeWindow = std::chrono::seconds(60); // Last minute
    auto now = std::chrono::steady_clock::now();
    
    size_t recentAllocations = 0;
    size_t failedAllocations = 0;
    
    for (const auto& event : state.recentEvents) {
        if (now - event.timestamp <= timeWindow) {
            if (event.type == Memory::EventType::Allocation) {
                recentAllocations++;
                if (!event.succeeded) {
                    failedAllocations++;
                }
            }
        }
    }
    
    metrics.allocationRate = (float)recentAllocations / 60.0f; // Per second
    metrics.failureRate = recentAllocations > 0 ? 
        (float)failedAllocations / recentAllocations : 0.0f;
    
    // Detect page thrashing
    metrics.hasPageThrashing = detectPageThrashing(state);
    
    return metrics;
}

PressureLevel MemoryPressureAnalyzer::analyzePressure(const Memory::MemoryState& state) {
    auto metrics = calculateMetrics(state);
    
    // Weighted scoring system
    float pressureScore = 0.0f;
    
    // Fragmentation impact (30%)
    pressureScore += metrics.fragmentationRatio * 0.3f;
    
    // Page utilization impact (25%)
    pressureScore += metrics.pageUtilization * 0.25f;
    
    // Allocation failure impact (25%)
    pressureScore += metrics.failureRate * 0.25f;
    
    // Page thrashing impact (20%)
    if (metrics.hasPageThrashing) {
        pressureScore += 0.2f;
    }
    
    // Determine pressure level
    if (pressureScore >= m_CriticalThreshold) return PressureLevel::Critical;
    if (pressureScore >= m_HighThreshold) return PressureLevel::High;
    if (pressureScore >= m_MediumThreshold) return PressureLevel::Medium;
    return PressureLevel::Low;
}

}} // namespace Hydragon::Tools 