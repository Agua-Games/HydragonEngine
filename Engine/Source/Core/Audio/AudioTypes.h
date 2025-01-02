/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Audio system type definitions
 */

#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Transform.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Audio {

using SoundHandle = uint32_t;
using SourceHandle = uint32_t;
using EffectHandle = uint32_t;

enum class SoundType {
    Effect,
    Music,
    Voice,
    Ambient
};

enum class AudioFormat {
    PCM16,
    PCM32Float,
    Vorbis,
    MP3,
    ADPCM
};

struct SoundDesc {
    std::string path;
    SoundType type;
    bool streaming = false;
    bool looping = false;
    float volume = 1.0f;
    float pitch = 1.0f;
    float minDistance = 1.0f;
    float maxDistance = 100.0f;
};

struct PlayParams {
    bool spatial = true;
    float fadeInTime = 0.0f;
    float startTime = 0.0f;
    Vector3 position = {0, 0, 0};
    float priority = 0.0f;
};

enum class EffectType {
    Reverb,
    Echo,
    Filter,
    Distortion,
    Chorus
};

struct EffectDesc {
    EffectType type;
    float wetDryMix = 0.5f;
    std::vector<float> parameters;
};

struct AudioStats {
    uint32_t activeSources;
    uint32_t loadedSounds;
    uint32_t activeEffects;
    uint32_t streamingBuffers;
    float cpuUsage;
    size_t memoryUsed;
    size_t streamingMemoryUsed;
};

} // namespace Hydragon::Audio 