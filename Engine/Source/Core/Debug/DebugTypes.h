/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Debug system type definitions
 */

#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Color.h"
#include <cstdint>
#include <string>
#include <functional>
#include <chrono>

namespace Hydragon::Debug {

enum class LogLevel {
    Trace,
    Info,
    Warning,
    Error,
    Fatal
};

struct LogEntry {
    LogLevel level;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::string file;
    uint32_t line;
};

using WatchCallback = std::function<std::string()>;

struct WatchEntry {
    std::string name;
    WatchCallback callback;
    bool enabled;
};

struct ProfilerScope {
    uint64_t id;
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
};

struct DebugStats {
    uint32_t activeDrawCalls;
    uint32_t totalLogEntries;
    uint32_t activeWatches;
    uint32_t activeProfilerScopes;
    size_t memoryUsed;
};

} // namespace Hydragon::Debug 