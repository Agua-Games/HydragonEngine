/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime gameplay recording and replay management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeReplayConfig {
    bool enableCompression = true;
    bool enableDeltaEncoding = true;
    bool enableKeyframeGeneration = true;
    bool enableTimelineMarkers = true;
    uint32_t keyframeInterval = 300;  // frames
    uint32_t bufferSizeMB = 64;
    float recordingFidelity = 1.0f;  // 1.0 = full fidelity
    std::string replayDirectory = "Replays";
};

class RuntimeReplaySystem {
public:
    static RuntimeReplaySystem& Get();
    
    void Initialize(const RuntimeReplayConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ReplayHandle StartRecording(const ReplayDesc& desc);
    void PauseRecording(ReplayHandle handle);
    void ResumeRecording(ReplayHandle handle);
    void StopRecording(ReplayHandle handle);
    
    void StartPlayback(const std::string& replayFile);
    void PausePlayback();
    void ResumePlayback();
    void StopPlayback();
    
    void SetPlaybackSpeed(float speed);
    void SeekToTime(float timeSeconds);
    void SeekToMarker(const std::string& markerName);
    
    void AddReplayMarker(const std::string& name, const MarkerData& data = {});
    void RegisterReplayHandler(ReplayEventType type, const ReplayEventHandler& handler);
    void UnregisterReplayHandler(ReplayEventType type);
    
    void ExportReplay(ReplayHandle handle, const std::string& path);
    ReplayHandle ImportReplay(const std::string& path);
    
    ReplayState GetReplayState() const;
    float GetCurrentTime() const;
    const RuntimeReplayStats& GetStats() const { return m_Stats; }

private:
    RuntimeReplaySystem() = default;
    
    RuntimeReplayConfig m_Config;
    RuntimeReplayStats m_Stats;
    std::unordered_map<ReplayHandle, ReplayInstance> m_Replays;
    std::unordered_map<ReplayEventType, ReplayEventHandler> m_EventHandlers;
    std::vector<ReplayMarker> m_Markers;
    std::unique_ptr<IReplayProvider> m_Provider;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 