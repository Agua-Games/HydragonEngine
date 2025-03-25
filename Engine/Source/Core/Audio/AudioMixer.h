/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioMixer.h
 * @brief Header file for the AudioMixer class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioMixer is a class that represents an audio mixer node in the engine's node graph.
 * - It is used to mix and process audio signals.
 * - It uses the FMOD library for audio processing.
 * - As other nodes with time-varying features in Hydragon, AudioMixer leverages the agnostic nature of the Montage system for animating values, blending clips,
 * non-linear editing with multiple tracks, also procedural generation of data and operations, among others.
 */
#pragma once
//#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include "Node.h"
//#include "AudioTypes.h"
#include "Montage.h"
#include "AudioManager.h"
#include "Wave.h"

namespace hd {

struct AudioMixerInfo : public NodeInfo {
    AudioMixerInfo() {
        NodeType = "Audio/AudioMixer";
        inputs = {
            "AudioClips",    // Array of audio clips
            "AudioSettings"  // Audio settings
        };
        outputs = {
            "MixedAudio",    // Mixed audio signal
            "AudioMetrics"   // Audio performance metrics
        };
    }
};

class AudioMixer : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AudioMixer(const AudioMixerInfo& info = AudioMixerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AudioMixer() = default;     // Default destructor
};

} // namespace hd
