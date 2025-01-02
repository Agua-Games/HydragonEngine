/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Experimental network conditions simulation system
 */

#pragma once
#include "NetworkTypes.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::DevTools::Experimental {

struct NetworkSimConfig {
    bool enableLatencySimulation = true;
    bool enablePacketLoss = true;
    bool enableBandwidthLimiting = true;
    bool enableJitterSimulation = true;
    float baseLatency = 50.0f;  // ms
    float packetLossRate = 0.01f;  // 1%
    size_t bandwidthLimit = 1024 * 1024;  // 1 MB/s
};

class NetworkSimulator {
public:
    static NetworkSimulator& Get();
    
    void Initialize(const NetworkSimConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void SetLatencyProfile(const LatencyProfile& profile);
    void SetPacketLossProfile(const PacketLossProfile& profile);
    void SetBandwidthProfile(const BandwidthProfile& profile);
    
    void StartCapture(const std::string& name);
    void StopCapture();
    void AnalyzeCapture(const std::string& name);
    
    const NetworkSimStats& GetStats() const { return m_Stats; }

private:
    NetworkSimulator() = default;
    
    NetworkSimConfig m_Config;
    NetworkSimStats m_Stats;
    std::vector<NetworkCapture> m_Captures;
    std::unique_ptr<INetworkSimulator> m_Simulator;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Experimental 