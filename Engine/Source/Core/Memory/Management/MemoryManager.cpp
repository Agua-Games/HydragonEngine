/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory manager for Hydragon
 */

#include "MemoryManager.h"
#include <cassert>

namespace Hydragon {
namespace Memory {

void* MemoryManager::Allocate(size_t size, size_t alignment) {
    assert(size > 0 && "Allocation size must be greater than 0");
    
    // Try current strategy first
    if (m_CurrentStrategy) {
        void* ptr = m_CurrentStrategy->Allocate(size, alignment);
        if (ptr) {
            TrackAllocation(ptr, size, alignment);
            return ptr;
        }
    }
    
    // Fallback to default allocation
    void* ptr = BaseAllocate(size, alignment);
    if (ptr) {
        TrackAllocation(ptr, size, alignment);
    }
    
    return ptr;
}

void MemoryManager::TrackAllocation(void* ptr, size_t size, size_t alignment) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    m_TotalAllocated += size;
    
    #if HD_DEBUG
        MemoryBlock block;
        block.data = ptr;
        block.size = size;
        block.alignment = alignment;
        block.strategy = m_CurrentStrategy;
        m_AllocationMap[ptr] = block;
    #endif
}

}} // namespace Hydragon::Memory 