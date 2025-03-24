/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioManager.h
 * @brief Header file for the AudioManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioManager is a singleton class that manages the audio playback.
 * - It is responsible for playing, pausing, stopping, and managing the audio system.
 * - It uses the FMOD library for audio playback.
 * 
 */
#pragma once
#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include "Node.h"

namespace hd {

struct AudioManagerInfo : public NodeInfo {
    AudioManagerInfo() {
        NodeType = "Audio/AudioManager";
        inputs = {
            "AudioClips",    // Array of audio clips
            "AudioSettings"  // Audio settings
        };
        outputs = {
            "AudioStatus",   // Audio playback status
            "AudioMetrics"   // Audio performance metrics
        };
    }
};

class AudioManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    static AudioManager* getInstance();    // Get the singleton instance of the AudioManager.

    // These are called once, when the engine is initialized.
    void initialize() override;
    void load() override;

    // === Processing ===
    void processNodeGraph() override;     // This is called every frame. It updates the audio system.
    void update();

    // === Cleanup ===
    // These are called once, when the engine is cleaned up.
    void unload();
    void cleanup();
    ~AudioManager();
};

} // namespace hd
