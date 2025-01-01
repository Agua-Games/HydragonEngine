#pragma once
#include <QObject>
#include "StreamingMonitor.h"

namespace Hydragon {
namespace Tools {

class RealTimeMonitor : public QObject {
    Q_OBJECT
public:
    struct MonitoringConfig {
        bool trackStreaming = true;
        bool trackFragmentation = true;
        int updateIntervalMs = 1000;
        size_t maxEventsStored = 1000;
    };
    
    void startMonitoring(const MonitoringConfig& config = MonitoringConfig());
    void stopMonitoring();
    
signals:
    void memoryStateUpdated(const MemoryState& state);
    void streamingStateUpdated(const StreamingState& state);
    void fragmentationUpdated(const FragmentationInfo& info);
    void anomalyDetected(const MemoryAnomaly& anomaly);
    
private:
    void updateMemoryState();
    void checkForAnomalies();
    
    std::unique_ptr<StreamingMonitor> m_StreamingMonitor;
    CircularBuffer<MemoryState> m_RecentStates;
    MonitoringConfig m_Config;
};

}} // namespace Hydragon::Tools 