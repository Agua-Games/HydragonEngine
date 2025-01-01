/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory management for node execution and hot reloading
 */

#pragma once
#include "ScriptingMemoryStrategy.h"

namespace Hydragon {
namespace Memory {

class NodeExecutionMemory {
public:
    // Called when executing node/script via editor action
    void* allocateForHotReload(const char* scriptId, size_t size) {
        ScriptingMemoryStrategy::ScriptExecutionHint hint{
            .isHotReloadable = true,
            .isCoroutine = false,
            .isPersistentState = false,
            .nodeGraphId = getGraphIdForScript(scriptId)
        };

        void* ptr = m_Strategy.allocateForScript(size, hint);
        m_HotReloadTracker.trackAllocation(ptr, scriptId);
        return ptr;
    }

    // For async node execution
    void* allocateForCoroutine(size_t size, uint32_t nodeId) {
        ScriptingMemoryStrategy::ScriptExecutionHint hint{
            .isHotReloadable = false,
            .isCoroutine = true,
            .isPersistentState = false,
            .nodeGraphId = getGraphIdForNode(nodeId)
        };

        return m_Strategy.allocateForScript(size, hint);
    }

    // Handle script/node recompilation
    void onScriptRecompiled(const char* scriptId) {
        // Invalidate old memory but preserve persistent state
        auto oldAllocations = m_HotReloadTracker.getAllocationsForScript(scriptId);
        for (auto& alloc : oldAllocations) {
            if (!alloc.isPersistentState) {
                m_Strategy.deallocate(alloc.ptr);
            }
        }
    }

private:
    ScriptingMemoryStrategy m_Strategy;
    
    struct HotReloadTracker {
        struct Allocation {
            void* ptr;
            bool isPersistentState;
        };
        std::unordered_map<std::string, std::vector<Allocation>> m_ScriptAllocations;
    } m_HotReloadTracker;
};

}} // namespace Hydragon::Memory 