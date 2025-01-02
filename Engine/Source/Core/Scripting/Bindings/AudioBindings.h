/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Audio system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Audio/AudioSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct AudioBindingsConfig {
    bool enableSpatialization = true;
    bool enableEffects = true;
    bool enableStreamedAudio = true;
    bool enableVisualization = true;
    uint32_t maxSoundInstances = 100;
    uint32_t maxEffectInstances = 50;
    uint32_t streamingBufferSize = 32768;  // 32KB
    std::string audioDataPath = "Data/Audio";
};

class AudioBindings {
public:
    static AudioBindings& Get();
    
    void Initialize(const AudioBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Sound management
    SoundHandle CreateSound(const std::string& name, const SoundDesc& desc);
    void DestroySound(SoundHandle handle);
    void PreloadSound(const std::string& name);
    
    // Playback control
    SourceHandle PlaySound(const std::string& name, const PlayParams& params = {});
    void StopSound(SourceHandle handle);
    void PauseSound(SourceHandle handle);
    void ResumeSound(SourceHandle handle);
    
    // Sound properties
    void SetVolume(const std::string& name, float volume);
    void SetPitch(const std::string& name, float pitch);
    void SetPosition(SourceHandle handle, const Vector3& position);
    
    // Effect management
    EffectHandle CreateEffect(const std::string& name, const EffectDesc& desc);
    void DestroyEffect(EffectHandle handle);
    void ApplyEffect(const std::string& soundName, const std::string& effectName);
    
    // Listener control
    void SetListenerPosition(const Vector3& position);
    void SetListenerOrientation(const Vector3& forward, const Vector3& up);
    
    // Audio visualization
    void EnableVisualization(bool enable);
    void GetSpectrum(std::vector<float>& spectrum);
    void GetWaveform(std::vector<float>& waveform);
    
    const AudioBindingsStats& GetStats() const { return m_Stats; }

private:
    AudioBindings() = default;
    
    AudioBindingsConfig m_Config;
    AudioBindingsStats m_Stats;
    std::unordered_map<std::string, SoundHandle> m_SoundMap;
    std::unordered_map<std::string, EffectHandle> m_EffectMap;
    std::unordered_map<SourceHandle, SoundInstance> m_ActiveSounds;
    std::unique_ptr<Audio::IAudioProcessor> m_Processor;
    std::unique_ptr<Audio::IEffectProcessor> m_EffectProcessor;
    std::unique_ptr<Audio::IVisualizer> m_Visualizer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 