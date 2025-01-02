/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Task management system
 */

#pragma once
#include "TaskTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Tasks {

class TaskSystem {
public:
    struct TaskConfig {
        uint32_t maxTasks = 10000;
        uint32_t maxDependencies = 1000;
        bool enablePrioritization = true;
        bool enableWorkStealing = true;
        bool enableProfiling = true;
        size_t memoryBudget = 32 * 1024 * 1024;  // 32MB
    };

    static TaskSystem& Get();
    
    void Initialize(const TaskConfig& config = {});
    void Shutdown();
    
    void Update();
    
    TaskHandle CreateTask(const TaskDesc& desc);
    void DestroyTask(TaskHandle handle);
    
    void SubmitTask(TaskHandle handle);
    void WaitForTask(TaskHandle handle);
    
    void AddDependency(TaskHandle dependent, TaskHandle dependency);
    void RemoveDependency(TaskHandle dependent, TaskHandle dependency);
    
    bool IsTaskComplete(TaskHandle handle) const;
    float GetTaskProgress(TaskHandle handle) const;
    
    const TaskStats& GetStats() const { return m_Stats; }

private:
    TaskSystem() = default;
    
    TaskConfig m_Config;
    TaskStats m_Stats;
    std::vector<Task> m_Tasks;
    std::unique_ptr<ITaskScheduler> m_Scheduler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Tasks 