/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory timeline system for Hydragon
 */

#pragma once
#include <QObject>
#include "MemorySnapshot.h"
#include <deque>

namespace Hydragon {
namespace Tools {

class MemoryTimelineSystem : public QObject {
    Q_OBJECT
public:
    struct TimelineConfig {
        size_t maxSnapshots = 1000;
        std::chrono::seconds snapshotInterval{1};
        bool autoCapture = true;
    };

    void startRecording(const TimelineConfig& config = TimelineConfig());
    void stopRecording();
    
    const MemorySnapshot* getSnapshot(size_t index) const;
    std::pair<size_t, size_t> findTimeRange(const QDateTime& start, 
                                           const QDateTime& end) const;

signals:
    void snapshotAdded(const MemorySnapshot& snapshot);
    void timelineUpdated();

private:
    std::deque<MemorySnapshot> m_History;
    TimelineConfig m_Config;
    QTimer* m_CaptureTimer;
    
    void captureSnapshot();
    void pruneOldSnapshots();
};

}} // namespace Hydragon::Tools 