/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioFile.h
 * @brief Header file for the AudioFile class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioFile is a class that represents an audio file in the engine's node graph.
 * - It is used to load and manage audio files.
 * - It uses the FMOD library for audio file management.
 * - To understand the Audio system, the function of each node:
 * 
 *      AudioFile --> AudioClip (for settings, some FX) --> AudioProcessor (for FX, modulation. Or use EQ, etc.) --> Montage --> AudioManager
 * 
 */
#pragma once
//#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "PhysicsTypes.h"
#include "AcousticProcessor.h"
/* #include "AudioPool.h"           // Nice suggestions for interfaces
#include "WavePool.h"
#include "AudioAsset.h"
#include "AudioConfig.h"
#include "AudioEvent.h"
#include "AudioTypes.h" */

namespace hd {

struct AudioFileInfo : public Resource {
    AudioFileInfo() {
        NodeType = "Audio/AudioFile";
        inputs = {
            "AudioPath",     // Path to the audio file
            "AudioData",     // Audio data
            "AudioMetadata"  // Audio metadata
        };
        outputs = {
            "LoadedAudio",   // Loaded audio file
            "AudioStatus",   // Audio loading status
            "AudioMetadata"  // Audio metadata
        }
    }
};

class AudioFile : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AudioFile(const AudioFileInfo& info = AudioFileInfo())
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
    ~AudioFile() = default;     // Default destructor
};

} // namespace hd
