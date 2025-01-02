/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Experimental scripting debugging and analysis tools
 */

#pragma once
#include "ScriptTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::DevTools::Experimental {

struct ScriptDebuggerConfig {
    bool enableLiveDebugging = true;
    bool enableHotReload = true;
    bool enablePerformanceAnalysis = true;
    bool enableMemoryTracking = true;
    uint32_t maxBreakpoints = 1000;
    uint32_t maxWatchVariables = 100;
};

class ScriptDebugger {
public:
    static ScriptDebugger& Get();
    
    void Initialize(const ScriptDebuggerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void AttachToScript(ScriptHandle handle);
    void DetachFromScript(ScriptHandle handle);
    
    void AddBreakpoint(const ScriptLocation& location);
    void RemoveBreakpoint(const ScriptLocation& location);
    
    void AddWatch(const std::string& expression);
    void RemoveWatch(const std::string& expression);
    
    void StepInto();
    void StepOver();
    void StepOut();
    void Continue();
    
    const ScriptDebuggerStats& GetStats() const { return m_Stats; }

private:
    ScriptDebugger() = default;
    
    ScriptDebuggerConfig m_Config;
    ScriptDebuggerStats m_Stats;
    std::vector<Breakpoint> m_Breakpoints;
    std::unordered_map<ScriptHandle, DebugContext> m_DebugContexts;
    std::unique_ptr<IScriptDebugger> m_Debugger;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Experimental 