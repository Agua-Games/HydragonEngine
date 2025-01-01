#pragma once
#include <cstddef>
#include <chrono>
#include <string>

namespace Hydragon {
namespace Memory {

enum class ModulePriority {
    Critical,   // System-critical modules (never evicted)
    High,       // Important gameplay modules
    Normal,     // Standard modules
    Low         // Background/optional modules
};

enum class AllocationStrategy {
    Pool,       // Fixed-size pool allocations
    Heap,       // General heap allocations
    Stack,      // Stack-based allocations
    Streaming   // Streaming memory
};

struct AllocationInfo {
    std::string tag;
    AllocationStrategy strategy;
    ModulePriority priority;
    size_t alignment;
    bool isTemporary;
};

struct MemoryStats {
    size_t totalAllocated;
    size_t currentUsage;
    size_t peakUsage;
    size_t allocationCount;
    std::chrono::steady_clock::time_point lastUpdate;
};

}} // namespace Hydragon::Memory 