#pragma once
#include "Core/Common/Profiling.h"

namespace Hydragon {
namespace Memory {

// Lightweight memory operation tracking
#define HD_TRACK_ALLOC(size) \
    MemoryProfiler::Get().trackAllocation(size, HD_CURRENT_LOCATION)

#define HD_TRACK_DEALLOC(ptr) \
    MemoryProfiler::Get().trackDeallocation(ptr)

// Memory metrics collection
struct MemoryMetrics {
    struct AllocationStats {
        size_t count;
        size_t totalSize;
        double avgLatency;
    };
    
    AllocationStats lastSecond;
    AllocationStats lastMinute;
    size_t peakUsage;
};

}} // namespace Hydragon::Memory 