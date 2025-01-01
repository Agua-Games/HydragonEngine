/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * NUMA-aware memory allocation for Hydragon
 */

#pragma once
#include "NumaAllocator.h"
#include "Core/Threading/ThreadAffinity.h"

namespace Hydragon {
namespace Memory {

class NumaAwareAllocator : public NumaAllocator {
public:
    // Specialized allocation hints for different editor scenarios
    struct EditorNumaHint {
        enum class WorkloadType {
            SceneEditing,    // Scene graph operations, selection
            AssetProcessing, // Texture/mesh processing
            Simulation,      // Physics, particles
            UI              // Editor interface
        } workload;
        
        bool isPersistent;   // Long-lived data
        bool isHotPath;      // Frequently accessed
        uint32_t threadGroup; // For thread affinity
    };

    void* allocateForEditor(size_t size, const EditorNumaHint& hint) {
        AllocationInfo info{};
        
        switch (hint.workload) {
            case EditorNumaHint::WorkloadType::SceneEditing:
                // Scene data should be close to UI thread
                info.numaNode = getUIThreadNode();
                info.priority = AllocationPriority::High;
                break;
                
            case EditorNumaHint::WorkloadType::AssetProcessing:
                // Distribute across nodes for parallel processing
                info.numaNode = getNextAssetNode();
                info.priority = AllocationPriority::Normal;
                break;
                
            case EditorNumaHint::WorkloadType::Simulation:
                // Bind to simulation thread nodes
                info.numaNode = getSimulationNode(hint.threadGroup);
                info.priority = hint.isHotPath ? 
                    AllocationPriority::High : 
                    AllocationPriority::Normal;
                break;
                
            case EditorNumaHint::WorkloadType::UI:
                // Always on the UI node
                info.numaNode = m_UINode;
                info.priority = AllocationPriority::High;
                break;
        }

        return allocate(size, info);
    }

private:
    uint32_t getUIThreadNode() const {
        return m_UINode;
    }

    uint32_t getSimulationNode(uint32_t threadGroup) {
        // Round-robin assignment for simulation threads
        return m_SimulationNodes[threadGroup % m_SimulationNodes.size()];
    }

    uint32_t getNextAssetNode() {
        // Distribute asset processing across available nodes
        uint32_t node = m_CurrentAssetNode;
        m_CurrentAssetNode = (m_CurrentAssetNode + 1) % m_NumaNodes.size();
        return node;
    }

    std::vector<uint32_t> m_NumaNodes;
    std::vector<uint32_t> m_SimulationNodes;
    uint32_t m_UINode;
    std::atomic<uint32_t> m_CurrentAssetNode{0};
};

}} // namespace Hydragon::Memory 