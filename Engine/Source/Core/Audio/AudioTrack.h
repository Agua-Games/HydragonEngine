/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioTrack.h
 * @brief Header file for the AudioTrack class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioTrack is a class that represents an audio track in the engine's node graph.
 * - It is used to play and manage audio clips.
 * - It uses the FMOD library for audio playback.
 * 
 */
#pragma once
//#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsTypes.h"
#include "AcousticProcessor.h"
/* #include "AudioPool.h"           // Nice suggestions for interfaces
#include "WavePool.h"
#include "AudioAsset.h"
#include "AudioConfig.h"
#include "AudioEvent.h"
#include "AudioTypes.h" */

namespace hd {

struct AudioTrackInfo : public NodeInfo {
    AudioTrackInfo() {
        NodeType = "Audio/AudioTrack";
        inputs = {
            "AudioClip",     // Audio clip to play
            "PlaybackSpeed"  // Playback speed
        };
        outputs = {
            "AudioStatus",   // Audio playback status
            "AudioMetrics"   // Audio performance metrics
        };
    }
};

class AudioTrack : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AudioTrack(const AudioTrackInfo& info = AudioTrackInfo())
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
    ~AudioTrack() = default;     // Default destructor
};

} // namespace hd
