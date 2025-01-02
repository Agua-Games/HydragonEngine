/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime cinematic sequence and cutscene management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeCinematicConfig {
    bool enableBlending = true;
    bool enableTimeWarping = true;
    bool enableEventTracks = true;
    bool enableCameraOverrides = true;
    uint32_t maxActiveCinematics = 8;
    uint32_t maxTrackCount = 32;
    float defaultBlendTime = 0.5f;
    float defaultEaseInTime = 0.3f;
    std::string cinematicCachePath = "Cache/Cinematics";
};

class RuntimeCinematicSystem {
public:
    static RuntimeCinematicSystem& Get();
    
    void Initialize(const RuntimeCinematicConfig& config = {});
    void Shutdown();
    
    void Update();
    
    CinematicHandle CreateCinematic(const CinematicDesc& desc);
    void DestroyCinematic(CinematicHandle handle);
    
    void PlayCinematic(CinematicHandle handle, const PlaybackParams& params = {});
    void StopCinematic(CinematicHandle handle);
    void PauseCinematic(CinematicHandle handle);
    void ResumeCinematic(CinematicHandle handle);
    
    void SetCinematicTime(CinematicHandle handle, float time);
    void SetCinematicSpeed(CinematicHandle handle, float speed);
    void SetCinematicWeight(CinematicHandle handle, float weight);
    
    void AddTrack(CinematicHandle handle, const TrackDesc& desc);
    void RemoveTrack(CinematicHandle handle, TrackHandle track);
    void SetTrackEnabled(CinematicHandle handle, TrackHandle track, bool enabled);
    
    void SetCameraTarget(CinematicHandle handle, EntityHandle target);
    void SetCameraBlendMode(CinematicHandle handle, CameraBlendMode mode);
    
    void RegisterEventHandler(const std::string& event, const CinematicEventHandler& handler);
    void UnregisterEventHandler(const std::string& event);
    
    void SaveCinematicState(CinematicHandle handle, const std::string& name);
    void LoadCinematicState(CinematicHandle handle, const std::string& name);
    
    CinematicState GetCinematicState(CinematicHandle handle) const;
    float GetCinematicProgress(CinematicHandle handle) const;
    const RuntimeCinematicStats& GetStats() const { return m_Stats; }

private:
    RuntimeCinematicSystem() = default;
    
    RuntimeCinematicConfig m_Config;
    RuntimeCinematicStats m_Stats;
    std::unordered_map<CinematicHandle, CinematicInstance> m_Cinematics;
    std::unordered_map<std::string, CinematicEventHandler> m_EventHandlers;
    std::unordered_map<std::string, CinematicState> m_SavedStates;
    std::unique_ptr<ICinematicProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 