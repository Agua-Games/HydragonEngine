/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory debugging and analysis tools
 */

#pragma once
#include "../DebugTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::DevTools {

struct MemoryToolsConfig {
    bool enableLeakDetection = true;
    bool enableCorruptionDetection = true;
    bool enableAllocationTracking = true;
    bool enableHeapProfiling = true;
    size_t maxTrackingEntries = 100000;
    size_t stackTraceDepth = 32;
};

class MemoryTools {
public:
    static MemoryTools& Get();
    
    void Initialize(const MemoryToolsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void TakeSnapshot(const std::string& name);
    void CompareSnapshots(const std::string& snapshot1, const std::string& snapshot2);
    
    void EnableStackTracing(bool enable);
    void EnableLeakDetection(bool enable);
    
    void AnalyzeHeapFragmentation();
    void GenerateAllocationReport(const std::string& path);
    
    const MemoryToolsStats& GetStats() const { return m_Stats; }

private:
    MemoryTools() = default;
    
    MemoryToolsConfig m_Config;
    MemoryToolsStats m_Stats;
    std::unordered_map<std::string, MemorySnapshot> m_Snapshots;
    std::unique_ptr<IMemoryAnalyzer> m_Analyzer;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools 