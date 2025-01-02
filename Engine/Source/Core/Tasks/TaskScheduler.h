/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Task scheduling and management system
 */

#pragma once
#include "TaskTypes.h"
#include "Core/Threading/ThreadPool.h"
#include <memory>

namespace Hydragon::Tasks {

class TaskScheduler {
public:
    struct SchedulerConfig {
        uint32_t maxPendingTasks = 1024;
        uint32_t maxConcurrentTasks = 64;
        bool enableTaskSteal = true;
        bool trackDependencies = true;
    };

    explicit TaskScheduler(const SchedulerConfig& config = {});
    ~TaskScheduler();

    void Initialize();
    void Shutdown();

    TaskHandle ScheduleTask(const TaskDesc& desc);
    void WaitForTask(TaskHandle handle);
    void CancelTask(TaskHandle handle);

    const TaskStats& GetStats() const { return m_Stats; }
    bool IsTaskComplete(TaskHandle handle) const;

private:
    SchedulerConfig m_Config;
    TaskStats m_Stats;
    std::unique_ptr<Threading::ThreadPool> m_ThreadPool;
    
    void ProcessTaskQueue();
    void UpdateDependencies(TaskHandle completedTask);
};

} // namespace Hydragon::Tasks 