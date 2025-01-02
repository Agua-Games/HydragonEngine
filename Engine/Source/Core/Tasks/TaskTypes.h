/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Task system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

namespace Hydragon::Tasks {

using TaskHandle = uint32_t;
using TaskFunction = std::function<void()>;

enum class TaskPriority {
    Low,
    Normal,
    High,
    Critical
};

struct TaskDesc {
    std::string name;
    TaskFunction function;
    TaskPriority priority = TaskPriority::Normal;
    bool isAsync = false;
    float weight = 1.0f;  // For progress tracking
    std::vector<TaskHandle> dependencies;
};

struct TaskStats {
    uint32_t activeTasks;
    uint32_t completedTasks;
    uint32_t totalDependencies;
    float averageLatencyMs;
    float averageExecutionTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Tasks 