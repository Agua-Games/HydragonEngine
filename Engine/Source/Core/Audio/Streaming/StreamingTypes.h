/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Audio streaming type definitions
 */

#pragma once
#include <cstdint>
#include <vector>

namespace Hydragon::Audio {

struct StreamParams {
    uint32_t bufferCount = 3;
    size_t bufferSize = 256 * 1024;
    bool loop = false;
    float priority = 0.0f;
};

struct StreamRequest {
    SoundHandle handle;
    StreamParams params;
    uint64_t timestamp;
};

struct StreamingStats {
    uint32_t activeStreams;
    uint32_t pendingRequests;
    uint32_t bufferUnderruns;
    float averageLatencyMs;
    size_t totalBufferSize;
    size_t peakMemoryUsed;
};

} // namespace Hydragon::Audio 