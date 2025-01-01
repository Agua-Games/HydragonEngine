/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * NUMA-aware memory allocation for Hydragon
 */

#pragma once
#include "IMemoryStrategy.h"
#include "Core/Platform/NumaAPI.h"

namespace Hydragon {
namespace Memory {

class NumaAllocator : public IMemoryStrategy {
public:
    struct NumaConfig {
        bool preferLocalAllocation = true;
        bool strictNodeBinding = false;
        size_t nodeReserveSize = 64 * 1024 * 1024;  // 64MB per node
    };

    explicit NumaAllocator(const NumaConfig& config = {});

    void* allocate(size_t size, const AllocationInfo& info) override {
        uint32_t preferredNode = info.numaNode;
        
        if (m_Config.preferLocalAllocation) {
            // Get the node of the current thread
            preferredNode = Platform::GetCurrentNumaNode();
        }

        void* ptr = Platform::NumaAllocateOnNode(size, preferredNode);
        if (!ptr && !m_Config.strictNodeBinding) {
            // Fallback to any node if strict binding is not required
            ptr = Platform::NumaAllocateAny(size);
        }

        return ptr;
    }

private:
    NumaConfig m_Config;
    std::vector<void*> m_NodeMemory;  // Per-node memory pools
};

}} // namespace Hydragon::Memory 