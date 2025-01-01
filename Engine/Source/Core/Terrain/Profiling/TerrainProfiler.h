/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Performance profiling for terrain systems
 */

#pragma once
#include "Core/Profiling/Profiler.h"
#include <deque>

namespace Hydragon {
namespace Terrain {

class TerrainProfiler {
public:
    struct StreamingMetrics {
        double averageStreamingTime = 0.0;
        double peakStreamingTime = 0.0;
        uint32_t streamingOperations = 0;
        size_t totalBytesStreamed = 0;
    };

    struct LODMetrics {
        double averageLODUpdateTime = 0.0;
        uint32_t lodTransitions = 0;
        float averageLODLevel = 0.0f;
    };

    void beginFrame() {
        m_CurrentFrame.startTime = Time::now();
    }

    void endFrame() {
        m_CurrentFrame.endTime = Time::now();
        updateMetrics();
        m_FrameHistory.push_back(m_CurrentFrame);
        
        if (m_FrameHistory.size() > MAX_FRAME_HISTORY) {
            m_FrameHistory.pop_front();
        }
    }

    void trackStreamingOperation(double duration, size_t bytes) {
        m_StreamingMetrics.averageStreamingTime = 
            (m_StreamingMetrics.averageStreamingTime * m_StreamingMetrics.streamingOperations + duration) /
            (m_StreamingMetrics.streamingOperations + 1);
            
        m_StreamingMetrics.peakStreamingTime = 
            std::max(m_StreamingMetrics.peakStreamingTime, duration);
            
        m_StreamingMetrics.streamingOperations++;
        m_StreamingMetrics.totalBytesStreamed += bytes;
    }

private:
    static constexpr size_t MAX_FRAME_HISTORY = 300; // 5 seconds at 60 fps

    struct FrameData {
        Time::Point startTime;
        Time::Point endTime;
        std::vector<ProfileEvent> events;
    };

    FrameData m_CurrentFrame;
    std::deque<FrameData> m_FrameHistory;
    StreamingMetrics m_StreamingMetrics;
    LODMetrics m_LODMetrics;

    void updateMetrics();
};

}} // namespace Hydragon::Terrain 