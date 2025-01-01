#pragma once
#include "Core/Memory/MemoryBlock.h"
#include "Core/Memory/Streaming/StreamingTypes.h"
#include <vector>
#include <chrono>

namespace Hydragon {
namespace Tools {

class MemorySnapshot {
public:
    struct StreamingState {
        std::vector<StreamRequest> pendingRequests;
        std::vector<StreamHandle> activeStreams;
        double currentThroughput;
    };
    
    void capture();
    void save(const std::string& filename);
    bool load(const std::string& filename);
    
    // Comparison
    MemoryDiff compareWith(const MemorySnapshot& other) const;
    
    // Analysis
    void analyzeFragmentation() const;
    void analyzeStreamingEfficiency() const;
    
private:
    std::chrono::system_clock::time_point m_CaptureTime;
    std::vector<Memory::MemoryBlock> m_Blocks;
    StreamingState m_StreamingState;
    
    // Serialization helpers
    void serializeToJson(QJsonObject& json) const;
    void deserializeFromJson(const QJsonObject& json);
};

}} // namespace Hydragon::Tools 