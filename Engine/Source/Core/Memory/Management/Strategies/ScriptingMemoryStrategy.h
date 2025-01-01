#pragma once
#include "IMemoryStrategy.h"
#include "NUMA/NumaAwareAllocator.h"

namespace Hydragon {
namespace Memory {

class ScriptingMemoryStrategy : public IMemoryStrategy {
public:
    // Specialized for live scripting/node execution
    struct ScriptExecutionHint {
        bool isHotReloadable;     // Can be modified without restart
        bool isCoroutine;         // For async node execution
        bool isPersistentState;   // Maintains state between executions
        uint32_t nodeGraphId;     // For grouping related nodes
    };

    void* allocateForScript(size_t size, const ScriptExecutionHint& hint) {
        // Fast path for coroutines - they need quick allocation/deallocation
        if (hint.isCoroutine) {
            return m_CoroutinePool.allocate(size);
        }

        // Hot-reloadable code gets special treatment
        if (hint.isHotReloadable) {
            return allocateHotReloadableMemory(size, hint.nodeGraphId);
        }

        // Persistent state goes to stable memory regions
        if (hint.isPersistentState) {
            return m_StatePool.allocate(size);
        }

        return m_DefaultPool.allocate(size);
    }

private:
    // Special pool for hot-reloadable code that might change frequently
    struct HotReloadPool {
        void* allocate(size_t size, uint32_t graphId) {
            // Use NUMA-aware allocation for frequently accessed code
            NumaAwareAllocator::EditorNumaHint hint{
                .workload = NumaAwareAllocator::EditorNumaHint::WorkloadType::SceneEditing,
                .isPersistent = false,
                .isHotPath = true
            };
            
            return m_NumaAllocator.allocateForEditor(size, hint);
        }

        void invalidateGraph(uint32_t graphId) {
            // Mark memory for graph as reusable
            // This allows for quick reallocation during hot reload
        }

        NumaAwareAllocator m_NumaAllocator;
        std::unordered_map<uint32_t, std::vector<MemoryBlock>> m_GraphMemory;
    };

    // Fast pool for coroutines
    LockFreePool<1024> m_CoroutinePool;  // Fixed size for quick alloc/dealloc
    
    // Stable memory for persistent state
    PoolStrategy m_StatePool;
    
    // Default pool for temporary allocations
    DefaultMemoryStrategy m_DefaultPool;
    
    HotReloadPool m_HotReloadPool;
};

}} // namespace Hydragon::Memory 