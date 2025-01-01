/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Pool memory management strategy for Hydragon
 */

#include "PoolStrategy.h"
#include "Core/Profiling/MemoryProfiler.h"

namespace Hydragon {
namespace Memory {

PoolStrategy::PoolStrategy(const PoolConfig& config) : m_Config(config) {
    // Initialize pools
    initializePools();
}

void* PoolStrategy::allocate(size_t size, const AllocationInfo& info) {
    HD_PROFILE_SCOPE("PoolStrategy::Allocate");
    
    // Find appropriate pool
    Pool* pool = findPool(size);
    if (!pool) return nullptr;
    
    // Find free block
    for (size_t i = 0; i < pool->used.size(); ++i) {
        if (!pool->used[i]) {
            pool->used[i] = true;
            void* ptr = static_cast<char*>(pool->memory) + (i * pool->blockSize);
            m_Stats.currentUsage += pool->blockSize;
            m_Stats.allocationCount++;
            return ptr;
        }
    }
    
    return nullptr;
}

void PoolStrategy::deallocate(void* ptr) {
    HD_PROFILE_SCOPE("PoolStrategy::Deallocate");
    
    // Find owning pool
    for (auto& pool : m_Pools) {
        if (ownsBlock(pool, ptr)) {
            size_t index = getBlockIndex(pool, ptr);
            pool.used[index] = false;
            m_Stats.currentUsage -= pool.blockSize;
            return;
        }
    }
}

private:
void PoolStrategy::initializePools() {
    // Small pools
    createPool(m_Config.smallPoolSize, m_Config.poolCount * 4);
    // Medium pools
    createPool(m_Config.mediumPoolSize, m_Config.poolCount * 2);
    // Large pools
    createPool(m_Config.mediumPoolSize * 4, m_Config.poolCount);
}

Pool* PoolStrategy::findPool(size_t size) {
    for (auto& pool : m_Pools) {
        if (size <= pool.blockSize && !isPoolFull(pool)) {
            return &pool;
        }
    }
    return nullptr;
}

bool PoolStrategy::isPoolFull(const Pool& pool) const {
    return std::find(pool.used.begin(), pool.used.end(), false) 
           == pool.used.end();
}

}} // namespace Hydragon::Memory 