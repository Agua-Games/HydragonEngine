/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor action memory for Hydragon
 */

#pragma once
#include "ScriptingMemoryStrategy.h"

namespace Hydragon {
namespace Memory {

class EditorActionMemory {
public:
    enum class ActionType {
        ScriptExecution,
        NodeEvaluation,
        GraphUpdate,
        StateReset
    };

    struct ActionContext {
        ActionType type;
        const char* identifier;    // Script/node/graph ID
        bool preserveState;        // Whether to maintain state across updates
        void* userData;           // Additional context-specific data
    };

    void* allocateForAction(const ActionContext& context, size_t size) {
        ScriptingMemoryStrategy::ScriptExecutionHint hint{
            .isHotReloadable = isHotReloadableAction(context.type),
            .isCoroutine = requiresCoroutine(context.type),
            .isPersistentState = context.preserveState,
            .nodeGraphId = getContextGraphId(context)
        };

        void* ptr = m_Strategy.allocateForScript(size, hint);
        trackAllocation(ptr, context);
        return ptr;
    }

private:
    bool isHotReloadableAction(ActionType type) {
        switch (type) {
            case ActionType::ScriptExecution:
            case ActionType::NodeEvaluation:
                return true;
            default:
                return false;
        }
    }

    bool requiresCoroutine(ActionType type) {
        // Determine based on action type and configuration
        return m_Config.isAsyncEnabled && 
               (type == ActionType::NodeEvaluation || 
                type == ActionType::GraphUpdate);
    }

    ScriptingMemoryStrategy m_Strategy;
    // ... rest of implementation
};

}} // namespace Hydragon::Memory 