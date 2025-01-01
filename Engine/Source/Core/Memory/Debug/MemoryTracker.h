/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory tracking and debugging utilities for Hydragon
 */

#pragma once
#include <atomic>
#include <unordered_map>
#include <mutex>
#include "Core/Threading/ThreadId.h"

namespace Hydragon {
namespace Memory {

struct AllocationInfo {
    const char* file;
    int line;
    size_t size;
    std::thread::id threadId;
    uint64_t timestamp;
    uint32_t stackHash;  // Hash of callstack
};

class MemoryTracker {
public:
    static MemoryTracker& Get();

    void trackAllocation(void* ptr, size_t size, const char* file, int line);
    void trackDeallocation(void* ptr);
    
    // Memory corruption detection
    void addGuardBands(void* ptr, size_t size);
    bool validateGuardBands(void* ptr);
    
    // Memory leak detection
    void dumpLeaks();
    size_t getLeakCount() const;
    
    // Statistics
    struct Stats {
        std::atomic<size_t> totalAllocations{0};
        std::atomic<size_t> currentAllocations{0};
        std::atomic<size_t> totalBytes{0};
        std::atomic<size_t> peakBytes{0};
        std::atomic<size_t> failedAllocations{0};
    };
    
    const Stats& getStats() const { return m_Stats; }

private:
    MemoryTracker() = default;
    
    static constexpr uint32_t GUARD_PATTERN = 0xDEADBEEF;
    static constexpr size_t GUARD_SIZE = 8;
    
    std::mutex m_Mutex;
    std::unordered_map<void*, AllocationInfo> m_Allocations;
    Stats m_Stats;
};

// Macros for easy tracking
#if HD_DEBUG
    #define HD_TRACK_ALLOC(ptr, size) \
        MemoryTracker::Get().trackAllocation(ptr, size, __FILE__, __LINE__)
    #define HD_TRACK_FREE(ptr) \
        MemoryTracker::Get().trackDeallocation(ptr)
#else
    #define HD_TRACK_ALLOC(ptr, size)
    #define HD_TRACK_FREE(ptr)
#endif

}} // namespace Hydragon::Memory 