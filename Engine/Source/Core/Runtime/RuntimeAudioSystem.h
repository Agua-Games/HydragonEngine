/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime audio playback and mixing management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeAudioConfig {
    bool enableSpatialization = true;
    bool enableOcclusion = true;
    bool enableReverbZones = true;
    bool enableDynamicMixing = true;
    uint32_t maxVoices = 128;
    uint32_t mixerBufferSize = 2048;
    uint32_t sampleRate = 48000;
    float dopplerScale = 1.0f;
    std::string audioCachePath = "Cache/Audio";
};

class RuntimeAudioSystem {
public:
    static RuntimeAudioSystem& Get();
    
    void Initialize(const RuntimeAudioConfig& config = {});
    void Shutdown();
    
    void Update();
    
    AudioSourceHandle CreateSource(const AudioSourceDesc& desc);
    void DestroySource(AudioSourceHandle handle);
    
    void PlaySound(AudioSourceHandle handle, const PlaybackParams& params = {});
    void StopSound(AudioSourceHandle handle);
    void PauseSound(AudioSourceHandle handle);
    void ResumeSound(AudioSourceHandle handle);
    
    void SetSourcePosition(AudioSourceHandle handle, const Vector3& position);
    void SetSourceVelocity(AudioSourceHandle handle, const Vector3& velocity);
    void SetSourceVolume(AudioSourceHandle handle, float volume);
    void SetSourcePitch(AudioSourceHandle handle, float pitch);
    
    void CreateBus(const std::string& name, const AudioBusDesc& desc);
    void DestroyBus(const std::string& name);
    void SetBusVolume(const std::string& name, float volume);
    
    void AddReverbZone(const std::string& name, const ReverbZoneDesc& desc);
    void RemoveReverbZone(const std::string& name);
    void UpdateReverbZone(const std::string& name, const ReverbParams& params);
    
    void SetListenerTransform(const Transform& transform);
    void SetListenerVelocity(const Vector3& velocity);
    
    void RegisterEffectProcessor(const std::string& name, std::unique_ptr<IAudioProcessor> processor);
    void UnregisterEffectProcessor(const std::string& name);
    
    AudioSourceState GetSourceState(AudioSourceHandle handle) const;
    float GetSourcePlaybackPosition(AudioSourceHandle handle) const;
    const RuntimeAudioStats& GetStats() const { return m_Stats; }

private:
    RuntimeAudioSystem() = default;
    
    RuntimeAudioConfig m_Config;
    RuntimeAudioStats m_Stats;
    std::unordered_map<AudioSourceHandle, AudioSourceInstance> m_Sources;
    std::unordered_map<std::string, AudioBus> m_Buses;
    std::unordered_map<std::string, ReverbZone> m_ReverbZones;
    std::unordered_map<std::string, std::unique_ptr<IAudioProcessor>> m_Processors;
    std::unique_ptr<IAudioEngine> m_Engine;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 