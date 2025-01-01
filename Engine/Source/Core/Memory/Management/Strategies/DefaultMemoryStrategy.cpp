/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Default memory management strategy for Hydragon
 */

#include "DefaultMemoryStrategy.h"
#include "Core/Platform/Memory.h"
#include "Core/Threading/Lock.h"

namespace Hydragon {
namespace Memory {

DefaultMemoryStrategy::DefaultMemoryStrategy(const Config& config) {
    // Platform-specific aligned allocation
    m_HeapStart = Platform::AlignedAlloc(config.initialHeapSize, 
                                        Platform::GetPageSize());
    m_HeapSize = config.initialHeapSize;
    
    // Initialize with single free block
    m_Blocks.push_back({
        m_HeapStart,
        m_HeapSize,
        false,
        {}
    });
}

void* DefaultMemoryStrategy::allocate(size_t size, const AllocationInfo& info) {
    ScopedLock lock(m_Mutex);  // Thread safety
    
    // Align size for better cache coherency
    size = Platform::AlignSize(size, HD_CACHE_LINE);
    
    // Fast path: check last successful allocation point
    if (auto block = findFreeBlockFast(size)) {
        return allocateFromBlock(block, size, info);
    }
    
    // Normal path: find any free block
    if (auto block = findFreeBlock(size)) {
        return allocateFromBlock(block, size, info);
    }
    
    // Slow path: try defragmentation
    if (m_Config.enableDefrag) {
        compact();
        if (auto block = findFreeBlock(size)) {
            return allocateFromBlock(block, size, info);
        }
    }
    
    return nullptr;
}

private:
HeapBlock* DefaultMemoryStrategy::findFreeBlockFast(size_t size) {
    // Check last allocation point first
    if (m_LastAllocationBlock && 
        !m_LastAllocationBlock->used && 
        m_LastAllocationBlock->size >= size) {
        return m_LastAllocationBlock;
    }
    return nullptr;
}

void* DefaultMemoryStrategy::allocateFromBlock(
    HeapBlock* block, size_t size, const AllocationInfo& info) {
    
    // Split if block is too large
    if (block->size > size + sizeof(HeapBlock)) {
        splitBlock(block, size);
    }
    
    block->used = true;
    block->info = info;
    
    // Update stats
    m_Stats.currentUsage += size;
    m_Stats.totalAllocated += size;
    m_Stats.peakUsage = std::max(m_Stats.peakUsage, m_Stats.currentUsage);
    
    // Cache for fast path
    m_LastAllocationBlock = block;
    
    return block->memory;
} 