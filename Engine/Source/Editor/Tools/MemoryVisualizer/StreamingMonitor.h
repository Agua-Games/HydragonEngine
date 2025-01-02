/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Streaming monitor for Hydragon
 */

#pragma once
#include <QObject>
#include "Core/Memory/Streaming/StreamingTypes.h"

namespace Hydragon {
namespace Tools {

class StreamingMonitor : public QObject {
    Q_OBJECT
public:
    struct StreamingStats {
        size_t pendingRequests;
        size_t activeStreams;
        double throughputMBps;
        std::vector<StreamingEvent> recentEvents;
    };
    
    void startMonitoring();
    void pauseMonitoring();
    
signals:
    void streamingEventReceived(const StreamingEvent& event);
    void streamingStatsUpdated(const StreamingStats& stats);
    
private:
    void onStreamingEvent(const StreamingEvent& event);
    void updateStreamingStats();
    
    StreamingStats m_CurrentStats;
    QTimer* m_UpdateTimer;
};

}} // namespace Hydragon::Tools 