/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Output system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <chrono>
#include <functional>

namespace Hydragon::Output {

using OutputChannelId = uint32_t;
using TimePoint = std::chrono::system_clock::time_point;

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

struct SourceLocation {
    const char* file;
    const char* function;
    int line;
    
    SourceLocation(const char* f = nullptr, const char* fn = nullptr, int l = 0)
        : file(f), function(fn), line(l) {}
};

struct LogMessage {
    LogLevel level;
    TimePoint timestamp;
    std::string message;
    SourceLocation location;
    uint32_t threadId;
    std::string category;
};

struct LogFilter {
    LogLevel minLevel = LogLevel::Info;
    std::vector<std::string> includedCategories;
    std::vector<std::string> excludedCategories;
    std::function<bool(const LogMessage&)> customFilter;
};

struct OutputStats {
    uint64_t totalMessages;
    uint64_t errorCount;
    uint64_t warningCount;
    uint32_t droppedMessages;
    uint32_t activeChannels;
    size_t memoryUsed;
    float averageLatencyMs;
};

} // namespace Hydragon::Output 