/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core audio system and sound management
 */

#pragma once
#include "AudioTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Audio {

class AudioSystem {
public:
    struct AudioConfig {
        uint32_t maxSources = 1000;
        uint32_t maxBuffers = 5000;
        uint32_t maxEffects = 100;
        bool enableSpatialization = true;
        bool enableEffects = true;
        bool enableStreamingAudio = true;
        size_t streamingBufferSize = 32 * 1024 * 1024;  // 32MB
        uint32_t sampleRate = 48000;
        uint32_t channels = 2;
    };

    static AudioSystem& Get();
    
    void Initialize(const AudioConfig& config = {});
    void Shutdown();
    
    void Update();
    
    SoundHandle CreateSound(const SoundDesc& desc);
    void DestroySound(SoundHandle handle);
    
    SourceHandle PlaySound(SoundHandle sound, const PlayParams& params = {});
    void StopSound(SourceHandle source);
    void PauseSound(SourceHandle source);
    void ResumeSound(SourceHandle source);
    
    void SetListenerTransform(const Transform& transform);
    void SetSourceTransform(SourceHandle source, const Transform& transform);
    
    void SetVolume(SourceHandle source, float volume);
    void SetPitch(SourceHandle source, float pitch);
    
    EffectHandle CreateEffect(const EffectDesc& desc);
    void DestroyEffect(EffectHandle handle);
    void ApplyEffect(SourceHandle source, EffectHandle effect);
    
    const AudioStats& GetStats() const { return m_Stats; }

private:
    AudioSystem() = default;
    
    AudioConfig m_Config;
    AudioStats m_Stats;
    std::unique_ptr<IAudioDevice> m_Device;
    std::vector<Sound> m_Sounds;
    std::vector<Source> m_Sources;
    bool m_Initialized = false;
};

} // namespace Hydragon::Audio 