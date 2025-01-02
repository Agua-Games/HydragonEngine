/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core profiling and performance monitoring system
 */

#pragma once
#include "ProfilerTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>
#include <chrono>

namespace Hydragon::Profiling {

struct SystemMetrics {
    PerformanceTimeline timeline;
    ResourceUtilization resources;
    BottleneckAnalysis bottlenecks;
};

class ProfilerSystem {
public:
    struct ProfilerConfig {
        bool enableCPUProfiling = true;
        bool enableGPUProfiling = true;
        bool enableMemoryProfiling = true;
        bool enableThreadProfiling = true;
        bool enableNetworkProfiling = true;
        bool enableFileIOProfiling = true;
        uint32_t maxSampleCount = 10000;
        uint32_t maxFrameHistory = 300;
        size_t memoryBudget = 64 * 1024 * 1024;  // 64MB
    };

    static ProfilerSystem& Get();
    
    void Initialize(const ProfilerConfig& config = {});
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    ScopeHandle BeginScope(const char* name, ProfilerCategory category);
    void EndScope(ScopeHandle handle);
    
    void AddMarker(const char* name, ProfilerCategory category);
    void AddCounter(const char* name, float value);
    
    void BeginGPUScope(const char* name);
    void EndGPUScope();
    
    void SetThreadName(const std::string& name);
    void EnableCapture(bool enable);
    
    void ExportCapture(const std::string& path);
    void ClearCapture();
    
    const ProfilerStats& GetStats() const { return m_Stats; }

private:
    ProfilerSystem() = default;
    
    ProfilerConfig m_Config;
    ProfilerStats m_Stats;
    std::vector<ProfilerFrame> m_FrameHistory;
    std::unique_ptr<IProfilerBackend> m_Backend;
    bool m_Initialized = false;
};

} // namespace Hydragon::Profiling 