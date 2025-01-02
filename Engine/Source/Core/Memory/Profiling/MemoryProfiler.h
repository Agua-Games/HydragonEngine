/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory profiling and tracking system
 */

#pragma once
#include "Core/Memory/Types/MemoryTypes.h"
#include "Core/Profiling/ProfilerTypes.h"
#include <chrono>
#include <vector>

namespace Hydragon::Memory {

class MemoryProfiler {
public:
    struct AllocationRecord {
        void* address;
        size_t size;
        std::string tag;
        std::chrono::steady_clock::time_point timestamp;
        uint32_t threadId;
        std::string callstack;
    };

    struct ProfilerStats {
        size_t totalAllocations;
        size_t peakMemoryUsage;
        std::vector<AllocationRecord> largestAllocations;
        float fragmentationRatio;
        std::unordered_map<std::string, size_t> allocationsByTag;
    };

    static void Initialize();
    static void Shutdown();
    
    static void TrackAllocation(const AllocationRecord& record);
    static void UntrackAllocation(void* ptr);
    
    static ProfilerStats GetStats();
    static void ResetStats();

private:
    static void UpdateFragmentationMetrics();
    static void PruneAllocationHistory();
};

} // namespace Hydragon::Memory 