/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * GPU memory management for Hydragon
 */

#include "GPUMemoryManager.h"
#include <algorithm>

namespace Hydragon {
namespace Memory {

GPUMemoryManager::GPUAllocation GPUMemoryManager::AllocateGPUMemory(
    VkMemoryRequirements reqs, VkMemoryPropertyFlags props) {
    
    // Find suitable memory heap
    GPUMemoryHeap* targetHeap = nullptr;
    for (auto& heap : m_Heaps) {
        if (heap.size - heap.used >= reqs.size) {
            targetHeap = &heap;
            break;
        }
    }
    
    // Create new heap if needed
    if (!targetHeap) {
        GPUMemoryHeap newHeap;
        // Allocate new device memory
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = std::max(reqs.size, m_MinHeapSize);
        allocInfo.memoryTypeIndex = FindMemoryType(reqs.memoryTypeBits, props);
        
        VkResult result = vkAllocateMemory(m_Device, &allocInfo, nullptr, &newHeap.memory);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate GPU memory");
        }
        
        newHeap.size = allocInfo.allocationSize;
        newHeap.used = 0;
        m_Heaps.push_back(newHeap);
        targetHeap = &m_Heaps.back();
    }
    
    // Create allocation
    GPUAllocation allocation;
    allocation.memory = targetHeap->memory;
    allocation.size = reqs.size;
    allocation.offset = targetHeap->used;
    allocation.memoryType = FindMemoryType(reqs.memoryTypeBits, props);
    
    targetHeap->used += reqs.size;
    
    // Track allocation
    m_TotalGPUMemoryUsed += reqs.size;
    
    return allocation;
}

}} // namespace Hydragon::Memory 