/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioBank.h
 * @brief Header file for the AudioBank class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioBank is a class that represents an audio bank in the engine's node graph. We can load a single file using AudioFile or a bank using AudioBank.
 * - It is used to load and manage audio banks, which are collections of audio files.
 * - It is compatible with FMOD banks.
 * - It uses the FMOD library for audio bank management.
 * 
 */
#pragma once
//#include <fmod.hpp>           // Commented out until we properly include FMOD
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "AudioFile.h"
#include "PhysicsTypes.h"
#include "AudioTypes.h"

namespace hd {

struct AudioBankInfo : public NodeInfo {
    AudioBankInfo() {
        nodeType = "Audio/AudioBank";
        inputs = {
            "audioBankFile",        // Audio bank file
            "crossfadeTime",        // Crossfade time for audio bank transitions
            "audioBankParams"       // Audio bank parameters
        };
        outputs = {
            "audioBankStatus",      // Audio bank status
            "audioBankMetrics"      // Audio bank performance metrics
        };
    }
};

class AudioBank : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AudioBank(const AudioBankInfo& info = AudioBankInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    std::unordered_map<std::string, AudioFile*> audioFiles;
    float crossfadeTime = 0.0f;
    std::unordered_map<std::string, std::any> audioBankParams;

    // === Processing ===
    void processNode() override {
        audioFiles = getInputValue<std::unordered_map<std::string, AudioFile*>>("audioFiles");
        crossfadeTime = getInputValue<float>("crossfadeTime");
        audioBankParams = getInputValue<std::unordered_map<std::string, std::any>>("audioBankParams");
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AudioBank() = default;     // Default destructor
};

} // namespace hd
