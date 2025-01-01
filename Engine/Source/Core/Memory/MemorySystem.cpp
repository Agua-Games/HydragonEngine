/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory system for Hydragon
 */

#include "MemorySystem.h"
#include "Management/Strategies/DefaultMemoryStrategy.h"
#include "Management/Strategies/PoolStrategy.h"
#include "Core/Profiling/MemoryProfiler.h"

namespace Hydragon {
namespace Memory {

void MemorySystem::initialize(const MemoryConfig& config) {
    if (m_Initialized) return;

    // Create and register default strategies
    auto defaultStrategy = std::make_unique<DefaultMemoryStrategy>();
    auto poolStrategy = std::make_unique<PoolStrategy>();

    addStrategy("Default", std::move(defaultStrategy));
    addStrategy("Pool", std::move(poolStrategy));
    setDefaultStrategy("Default");

    // Initialize profiling if enabled
    if (config.enableProfiling) {
        MemoryProfiler::Get().initialize();
    }

    m_Initialized = true;
}

void* MemorySystem::allocate(size_t size, const AllocationInfo& info) {
    HD_ASSERT(m_Initialized, "Memory system not initialized!");
    HD_PROFILE_SCOPE("MemorySystem::Allocate");

    // Select appropriate strategy based on allocation info
    IMemoryStrategy* strategy = m_DefaultStrategy;
    if (info.strategy == AllocationStrategy::Pool) {
        strategy = m_Strategies["Pool"].get();
    }

    // Perform allocation
    void* ptr = strategy->allocate(size, info);
    if (!ptr) {
        // Handle allocation failure
        handleAllocationFailure(size, info);
        return nullptr;
    }

    return ptr;
}

void MemorySystem::addStrategy(const std::string& name, 
                             std::unique_ptr<IMemoryStrategy> strategy) {
    HD_ASSERT(strategy, "Invalid strategy");
    m_Strategies[name] = std::move(strategy);
}

MemoryStats MemorySystem::getSystemStats() const {
    MemoryStats total{};
    
    // Aggregate stats from all strategies
    for (const auto& [name, strategy] : m_Strategies) {
        const auto stats = strategy->getStats();
        total.totalAllocated += stats.totalAllocated;
        total.currentUsage += stats.currentUsage;
        total.peakUsage = std::max(total.peakUsage, stats.peakUsage);
        total.allocationCount += stats.allocationCount;
    }
    
    return total;
}

private:
void MemorySystem::handleAllocationFailure(size_t size, 
                                         const AllocationInfo& info) {
    // Try to free memory
    compact();
    
    // Log failure
    HD_LOG_ERROR("Memory allocation failed: size={}, tag={}", 
                 size, info.tag);
}

}} // namespace Hydragon::Memory 