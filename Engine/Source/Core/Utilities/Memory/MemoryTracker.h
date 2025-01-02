/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory allocation tracking and analysis
 */

#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Hydragon::Utilities {

struct MemoryConfig {
    bool enableTracking = true;
    bool enableStackTraces = true;
    bool enableLeakDetection = true;
    bool enableAllocationHistory = true;
    size_t maxTrackingEntries = 10000;
};

class MemoryTracker {
public:
    static MemoryTracker& Get();
    
    void Initialize(const MemoryConfig& config = {});
    void Shutdown();
    
    void* TrackAllocation(void* ptr, size_t size, const char* file, int line);
    void TrackDeallocation(void* ptr);
    
    void BeginScope(const std::string& name);
    void EndScope();
    
    void DumpStats();
    void CheckLeaks();
    
    const MemoryStats& GetStats() const { return m_Stats; }
    std::vector<AllocationInfo> GetAllocationHistory() const;

private:
    MemoryTracker() = default;
    
    struct AllocationEntry {
        size_t size;
        const char* file;
        int line;
        std::string scope;
        std::vector<void*> stackTrace;
    };

    MemoryConfig m_Config;
    MemoryStats m_Stats;
    std::unordered_map<void*, AllocationEntry> m_Allocations;
    std::vector<std::string> m_ScopeStack;
    bool m_Initialized = false;
};

} // namespace Hydragon::Utilities 