/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime execution and process management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeExecutionConfig {
    bool enableProcessIsolation = true;
    bool enableResourceTracking = true;
    bool enableCrashRecovery = true;
    bool enableMetrics = true;
    uint32_t maxProcesses = 32;
    uint32_t watchdogInterval = 1000;  // ms
    size_t processMemoryLimit = 1024 * 1024 * 1024;  // 1GB
};

class RuntimeExecutionSystem {
public:
    static RuntimeExecutionSystem& Get();
    
    void Initialize(const RuntimeExecutionConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ProcessHandle StartProcess(const ProcessDesc& desc);
    void StopProcess(ProcessHandle handle);
    
    void SuspendProcess(ProcessHandle handle);
    void ResumeProcess(ProcessHandle handle);
    
    void SetProcessPriority(ProcessHandle handle, ProcessPriority priority);
    void SetProcessAffinity(ProcessHandle handle, uint64_t affinityMask);
    
    ProcessState GetProcessState(ProcessHandle handle) const;
    const RuntimeExecutionStats& GetStats() const { return m_Stats; }

private:
    RuntimeExecutionSystem() = default;
    
    RuntimeExecutionConfig m_Config;
    RuntimeExecutionStats m_Stats;
    std::unordered_map<ProcessHandle, ProcessInstance> m_Processes;
    std::unique_ptr<IProcessManager> m_Manager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 