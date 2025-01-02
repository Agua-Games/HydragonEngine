/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core debugging system
 */

#pragma once
#include "DebugTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Debug {

class DebugSystem {
public:
    struct DebugConfig {
        bool enableLogging = true;
        bool enableProfiling = true;
        bool enableVisualization = true;
        bool enableMemoryTracking = true;
        size_t maxLogEntries = 10000;
        size_t memoryBudget = 16 * 1024 * 1024;  // 16MB
    };

    static DebugSystem& Get();
    
    void Initialize(const DebugConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void DrawLine(const Vector3& start, const Vector3& end, const Color& color);
    void DrawSphere(const Vector3& center, float radius, const Color& color);
    void DrawBox(const Vector3& min, const Vector3& max, const Color& color);
    
    void Log(LogLevel level, const std::string& message);
    void AddWatch(const std::string& name, const WatchCallback& callback);
    
    ProfilerScope BeginScope(const std::string& name);
    void EndScope(ProfilerScope scope);
    
    const DebugStats& GetStats() const { return m_Stats; }

private:
    DebugSystem() = default;
    
    DebugConfig m_Config;
    DebugStats m_Stats;
    std::vector<LogEntry> m_LogEntries;
    std::vector<WatchEntry> m_Watches;
    std::unique_ptr<IProfiler> m_Profiler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Debug 