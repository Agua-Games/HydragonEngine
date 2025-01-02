/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Script debugging and inspection system
 */

#pragma once
#include "ScriptTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct ScriptDebuggerConfig {
    bool enableVariableInspection = true;
    bool enableCallStack = true;
    bool enableWatches = true;
    bool enableHotReload = true;
    uint32_t maxWatchCount = 100;
    uint32_t maxCallStackDepth = 128;
    float updateInterval = 0.1f;  // seconds
};

class ScriptDebuggerSystem {
public:
    static ScriptDebuggerSystem& Get();
    
    void Initialize(const ScriptDebuggerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void AttachToScript(const std::string& path);
    void DetachFromScript(const std::string& path);
    
    void Continue();
    void StepOver();
    void StepInto();
    void StepOut();
    
    void AddWatch(const std::string& expression);
    void RemoveWatch(const std::string& expression);
    
    void EvaluateExpression(const std::string& expression);
    
    const CallStackFrame& GetCurrentFrame() const;
    const std::vector<Variable>& GetLocalVariables() const;
    
    const ScriptDebuggerStats& GetStats() const { return m_Stats; }

private:
    ScriptDebuggerSystem() = default;
    
    ScriptDebuggerConfig m_Config;
    ScriptDebuggerStats m_Stats;
    std::unique_ptr<IDebugger> m_Debugger;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 