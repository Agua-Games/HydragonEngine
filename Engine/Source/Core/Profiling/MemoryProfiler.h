#pragma once
#include "Core/Memory/MemorySystem.h"

namespace Hydragon {
namespace Profiling {

class MemoryProfiler {
public:
    struct ProfilePoint {
        const char* tag;
        size_t size;
        uint64_t timestamp;
        ThreadId threadId;
        NodeGraphId graphId;  // For tracking node-specific allocations
    };

    // Track node graph memory performance
    void beginNodeExecution(NodeId id) {
        m_CurrentNode = id;
        m_NodeStartTime = Time::Now();
    }

    void endNodeExecution() {
        auto duration = Time::Now() - m_NodeStartTime;
        updateNodeStats(m_CurrentNode, duration);
    }

private:
    struct NodeMemoryStats {
        size_t peakMemory;
        float averageAllocationTime;
        std::vector<ProfilePoint> hotSpots;
    };
};

}} // namespace Hydragon::Profiling 