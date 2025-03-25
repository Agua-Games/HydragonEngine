/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioClip.h
 * @brief Header file for the AudioClip class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioClip is a class that represents an audio clip in the engine's node graph.
 * - It is used to play and manage audio clips, which are loaded from audio files.
 * - It has some basic features, like playback speed, volume, pitch, and looping.
 * - It also has some procedural features, like randomization of playback speed, volume, pitch, and looping.
 * - To understand the Audio system, the function of each node:
 * 
 *      AudioFile --> AudioClip (for settings, some FX) --> AudioProcessor (for FX, modulation. Or use EQ, etc.) --> Montage --> AudioManager
 * 
 * - It uses the FMOD library for audio playback.
 * 
 */
#pragma once
//#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "AudioFile.h"
#include "PhysicsTypes.h"
#include "AcousticProcessor.h"
/* #include "AudioPool.h"           // Nice suggestions for interfaces
#include "WavePool.h"
#include "AudioAsset.h"
#include "AudioConfig.h"
#include "AudioEvent.h"
#include "AudioTypes.h" */

namespace hd {

struct AudioClipInfo : public NodeInfo {
    AudioClipInfo() {
        NodeType = "Audio/AudioClip";
        inputs = {
            "AudioFile",
            "PlaybackSpeed",
            "Volume",
            "Pitch",
            "Looping",         // Whether to loop the audio clip
            "Randomize",       // Whether to randomize the audio clip
            "RandomizeParams", // Randomization parameters
            "ClipParams"

        };
        outputs = {
            "AudioStatus",    // Audio playback status
            "AudioMetrics"    // Audio performance metrics
        };
    }
};

class AudioClip : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AudioClip(const AudioClipInfo& info = AudioClipInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    AudioFile* audio = nullptr;
    float playbackSpeed = 1.0f;
    float volume = 1.0f;
    float pitch = 1.0f;
    bool looping = false;

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AudioClip() = default;     // Default destructor
};

} // namespace hd
