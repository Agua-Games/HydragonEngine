/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime performance profiling and monitoring
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeProfileConfig {
    bool enableCPUProfiling = true;
    bool enableGPUProfiling = true;
    bool enableMemoryProfiling = true;
    bool enableThreadProfiling = true;
    uint32_t maxSampleCount = 1000;
    uint32_t samplingIntervalMs = 16;
    std::string outputPath = "Logs/Profile";
};

class RuntimeProfileSystem {
public:
    static RuntimeProfileSystem& Get();
    
    void Initialize(const RuntimeProfileConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ProfileScope BeginScope(const std::string& name);
    void EndScope(ProfileScope scope);
    
    void BeginFrame(const std::string& name);
    void EndFrame();
    
    void AddMarker(const std::string& name, const MarkerData& data = {});
    void AddCounter(const std::string& name, float value);
    
    void ExportProfile(const std::string& path = "");
    void ClearProfile();
    
    const RuntimeProfileStats& GetStats() const { return m_Stats; }

private:
    RuntimeProfileSystem() = default;
    
    RuntimeProfileConfig m_Config;
    RuntimeProfileStats m_Stats;
    std::vector<ProfileFrame> m_Frames;
    std::unordered_map<std::string, ProfileCounter> m_Counters;
    std::unique_ptr<IProfiler> m_Profiler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 