/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime inspection and debugging tools
 */

#pragma once
#include "../DebugTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::DevTools {

struct InspectorConfig {
    bool enableLiveEdit = true;
    bool enableTypeInfo = true;
    bool enableCallstack = true;
    bool enableMemoryView = true;
    bool enableHistoryTracking = true;
    uint32_t maxHistoryEntries = 1000;
    uint32_t maxWatchVariables = 100;
};

class InspectorSystem {
public:
    static InspectorSystem& Get();
    
    void Initialize(const InspectorConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void AddWatch(const std::string& name, const WatchCallback& callback);
    void RemoveWatch(const std::string& name);
    
    void AddBreakpoint(const std::string& location);
    void RemoveBreakpoint(const std::string& location);
    
    void InspectObject(const void* object, const TypeInfo& typeInfo);
    void ModifyVariable(const std::string& name, const Variable& value);
    
    void CaptureCallstack();
    void CaptureMemorySnapshot();
    
    const InspectorStats& GetStats() const { return m_Stats; }

private:
    InspectorSystem() = default;
    
    InspectorConfig m_Config;
    InspectorStats m_Stats;
    std::vector<WatchEntry> m_Watches;
    std::vector<Breakpoint> m_Breakpoints;
    std::unique_ptr<IDebugger> m_Debugger;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools 