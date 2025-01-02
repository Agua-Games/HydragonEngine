/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Profiler type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <chrono>

namespace Hydragon::Profiling {

using ScopeHandle = uint32_t;
using TimePoint = std::chrono::high_resolution_clock::time_point;
using Duration = std::chrono::nanoseconds;

enum class ProfilerCategory {
    CPU,
    GPU,
    Memory,
    Network,
    FileIO,
    Audio,
    Physics,
    Animation,
    Rendering,
    Custom
};

struct ProfilerEvent {
    const char* name;
    ProfilerCategory category;
    TimePoint startTime;
    Duration duration;
    uint32_t threadId;
    uint32_t depth;
    size_t memoryUsage;
};

struct ProfilerFrame {
    uint64_t frameNumber;
    TimePoint startTime;
    Duration cpuTime;
    Duration gpuTime;
    std::vector<ProfilerEvent> events;
    float frameTimeMs;
    size_t peakMemoryUsage;
};

struct ProfilerStats {
    uint32_t activeScopes;
    uint32_t totalEvents;
    uint32_t droppedEvents;
    float averageFrameTimeMs;
    float minFrameTimeMs;
    float maxFrameTimeMs;
    float averageCPUTimeMs;
    float averageGPUTimeMs;
    size_t peakMemoryUsage;
};

struct PerformanceTimeline {
    std::vector<TimelineEvent> events;
    double startTime;
    double endTime;
    uint32_t markerCount;
};

struct ResourceUtilization {
    float cpuUsage;
    float gpuUsage;
    size_t memoryUsage;
    float networkBandwidth;
    float diskIORate;
};

struct BottleneckAnalysis {
    std::vector<BottleneckInfo> detectedBottlenecks;
    float severityScore;
    std::string primaryBottleneck;
    uint32_t occurrenceCount;
};

} // namespace Hydragon::Profiling 