#pragma once
#include "Core/Memory/MemoryTypes.h"
#include "Core/Memory/Streaming/StreamingTypes.h"
#include <deque>

namespace Hydragon {
namespace Tools {

class AnomalyDetector {
public:
    struct AnomalyConfig {
        float memorySpikeTreshold = 1.5f;    // 50% increase
        float streamingFailureRate = 0.1f;   // 10% failures
        size_t historySamples = 100;
    };
    
    void addMemorySample(const MemoryState& state);
    void addStreamingSample(const StreamingState& state);
    
    std::vector<MemoryAnomaly> detectMemoryAnomalies();
    std::vector<StreamingAnomaly> detectStreamingAnomalies();
    
private:
    std::deque<MemoryState> m_MemoryHistory;
    std::deque<StreamingState> m_StreamingHistory;
    AnomalyConfig m_Config;
    
    bool isMemorySpike(const MemoryState& current, 
                      const MemoryState& previous) const;
    bool isStreamingAnomaly(const StreamingState& state) const;
};

}} // namespace Hydragon::Tools 