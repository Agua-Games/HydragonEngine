/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Streaming memory manager for Hydragon
 */

#include "StreamingMemoryManager.h"
#include "Core/Profiling/MemoryProfiler.h"

namespace Hydragon {
namespace Memory {

void StreamingMemoryManager::registerStreamingSystem(IStreamingSystem* system) {
    HD_ASSERT(system, "Invalid streaming system");
    m_StreamingSystem = system;
    
    // Register for streaming events
    system->onStreamRequest.connect(this, &StreamingMemoryManager::handleStreamRequest);
    system->onStreamComplete.connect(this, &StreamingMemoryManager::handleStreamComplete);
}

void* StreamingMemoryManager::allocateStreamingMemory(
    size_t size, const StreamingAllocationInfo& info) {
    
    HD_PROFILE_SCOPE("StreamingMemory::Allocate");
    
    // Check module limits
    auto it = m_Modules.find(info.moduleName);
    if (it == m_Modules.end()) {
        HD_LOG_ERROR("Module {} not registered for streaming", info.moduleName);
        return nullptr;
    }
    
    auto& module = it->second;
    if (module.currentUsage + size > module.reservedMemory) {
        // Try eviction if enabled
        if (m_Config.enablePrioritization) {
            evictLowerPriorityBlocks(module, size);
        }
        
        // Still no space?
        if (module.currentUsage + size > module.reservedMemory) {
            HD_LOG_WARNING("Module {} exceeded streaming memory limit", info.moduleName);
            return nullptr;
        }
    }
    
    // Allocate memory
    void* ptr = m_Strategy->allocate(size);
    if (!ptr) return nullptr;
    
    // Track allocation
    auto* block = new StreamingBlock{
        ptr,
        size,
        info.priority,
        info.moduleName,
        std::chrono::steady_clock::now()
    };
    
    module.blocks.push_back(block);
    module.currentUsage += size;
    
    return ptr;
}

void StreamingMemoryManager::updateStreamingState() {
    HD_PROFILE_SCOPE("StreamingMemory::Update");
    
    // Update streaming metrics
    for (auto& [name, module] : m_Modules) {
        float usage = (float)module.currentUsage / module.reservedMemory;
        if (usage > m_Config.evictionThreshold) {
            evictOldestBlocks(module);
        }
    }
    
    // Process pending streaming requests
    m_StreamingSystem->processRequests();
}

private:
void StreamingMemoryManager::evictLowerPriorityBlocks(
    ModuleInfo& module, size_t requiredSize) {
    
    // Sort blocks by priority and age
    std::sort(module.blocks.begin(), module.blocks.end(),
        [](const StreamingBlock* a, const StreamingBlock* b) {
            if (a->priority != b->priority)
                return a->priority < b->priority;
            return a->lastAccess < b->lastAccess;
        });
    
    // Evict until we have enough space
    size_t evicted = 0;
    while (evicted < requiredSize && !module.blocks.empty()) {
        auto* block = module.blocks.front();
        evicted += block->size;
        deallocateStreamingMemory(block->ptr);
        module.blocks.erase(module.blocks.begin());
    }
}

}} // namespace Hydragon::Memory 