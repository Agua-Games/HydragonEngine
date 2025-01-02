/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Performance profiling and monitoring system
 */

#pragma once
#include <memory>
#include <string>
#include <chrono>
#include <vector>

namespace Hydragon::Utilities {

struct ProfilerConfig {
    bool enableProfiling = true;
    bool enableThreadProfiling = true;
    bool enableGPUProfiling = true;
    bool enableMemoryProfiling = true;
    uint32_t maxSamples = 1000;
    float samplingInterval = 0.016f;  // 60Hz
};

class ProfilerSystem {
public:
    static ProfilerSystem& Get();
    
    void Initialize(const ProfilerConfig& config = {});
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void BeginScope(const std::string& name);
    void EndScope();
    
    void AddMarker(const std::string& name);
    void AddCounter(const std::string& name, float value);
    
    void BeginGPUScope(const std::string& name);
    void EndGPUScope();
    
    const ProfilerStats& GetStats() const { return m_Stats; }
    const std::vector<ProfileSample>& GetFrameSamples() const;

private:
    ProfilerSystem() = default;
    
    ProfilerConfig m_Config;
    ProfilerStats m_Stats;
    std::vector<ProfileScope> m_ScopeStack;
    std::vector<ProfileSample> m_Samples;
    std::chrono::high_resolution_clock::time_point m_FrameStart;
    bool m_Initialized = false;
};

} // namespace Hydragon::Utilities 