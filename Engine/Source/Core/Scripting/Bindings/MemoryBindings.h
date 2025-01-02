/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Memory/MemorySystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct MemoryBindingsConfig {
    bool enableAllocationTracking = true;
    bool enableMemoryProfiling = true;
    bool enableLeakDetection = true;
    bool enableSafetyChecks = true;
    uint32_t maxTrackingEvents = 1000;
    size_t profilerBufferSize = 1024 * 1024;  // 1MB
    std::string memoryStatsPath = "Stats/Memory";
};

class MemoryBindings {
public:
    static MemoryBindings& Get();
    
    void Initialize(const MemoryBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Memory allocation tracking
    void TrackAllocation(const Memory::AllocationInfo& info);
    void TrackDeallocation(const Memory::AllocationInfo& info);
    
    // Memory profiling
    void BeginMemoryScope(const std::string& name);
    void EndMemoryScope();
    
    // Memory statistics
    void ExportMemoryStats(const std::string& path);
    void ClearMemoryStats();
    
    // Memory debugging
    void EnableLeakDetection(bool enable);
    void EnableMemoryGuards(bool enable);
    
    // Memory analysis
    void AnalyzeFragmentation();
    void GenerateAllocationReport();
    
    const MemoryBindingsStats& GetStats() const { return m_Stats; }

private:
    MemoryBindings() = default;
    
    MemoryBindingsConfig m_Config;
    MemoryBindingsStats m_Stats;
    std::unordered_map<std::string, MemoryScopeInfo> m_Scopes;
    std::vector<Memory::AllocationEvent> m_TrackingEvents;
    std::unique_ptr<Memory::IMemoryProfiler> m_Profiler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 