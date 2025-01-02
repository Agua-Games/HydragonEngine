/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Advanced profiling and analysis tools
 */

#pragma once
#include "../DebugTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <chrono>

namespace Hydragon::DevTools {

struct ProfilerToolsConfig {
    bool enableTimeline = true;
    bool enableFlameGraph = true;
    bool enableStatistics = true;
    bool enableExport = true;
    uint32_t maxTimelineEvents = 10000;
    uint32_t maxStackDepth = 64;
};

class ProfilerTools {
public:
    static ProfilerTools& Get();
    
    void Initialize(const ProfilerToolsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void BeginCapture(const std::string& name);
    void EndCapture();
    
    void AddEvent(const std::string& name, ProfilerEventType type);
    void AddMarker(const std::string& name, const std::string& details = "");
    
    void GenerateReport(const std::string& path);
    void ExportTimeline(const std::string& path);
    void ExportFlameGraph(const std::string& path);
    
    const ProfilerToolsStats& GetStats() const { return m_Stats; }

private:
    ProfilerTools() = default;
    
    ProfilerToolsConfig m_Config;
    ProfilerToolsStats m_Stats;
    std::vector<ProfilerEvent> m_Events;
    std::unique_ptr<IProfilerAnalyzer> m_Analyzer;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools 