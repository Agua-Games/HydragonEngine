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
        nodeType = "Audio/AudioFile";
        inputs = {
            "audioPath",     // Path to the audio file
            "audioData",     // Audio data
            "audioMetadata"  // Audio metadata
        };
        outputs = {
            "loadedAudio",   // Loaded audio file
            "audioStatus",   // Audio loading status
            "audioMetadata"  // Audio metadata
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

    // Set default values
    std::string audioPath;
    std::unordered_map<std::string, std::any> audioData;
    std::unordered_map<std::string, std::any> audioMetadata;

    // === Processing ===
    void processNode() override {
        audioPath = getInputValue<std::string>("audioPath");
        audioData = getInputValue<std::unordered_map<std::string, std::any>>("audioData");
        audioMetadata = getInputValue<std::unordered_map<std::string, std::any>>("audioMetadata");
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AudioFile() = default;     // Default destructor
};

} // namespace hd
