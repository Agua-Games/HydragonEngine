/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AcousticSource.h
 * @brief Header file for the AcousticSource class.
 * 
 * ARCHITECTURAL NOTES:
 * - AcousticSource is a node class that represents a sound source with acoustic properties.
 * - It also provides methods to get the current position, orientation, and velocity of the source.
 */
#pragma once
//#include <fmod.hpp>
#include <string>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "PhysicsTypes.h"

namespace hd {

struct AcousticSourceInfo : public NodeInfo {
    AcousticSourceInfo() {
        nodeType = "Audio/AcousticSource";
        inputs = {
            "position",          // Wave emission point
            "orientation",       // Emission direction
            "velocity",          // For Doppler effect
            "wavePattern",       // Acoustic wave characteristics
            "medium",            // Propagation medium properties
            "volumeMultiplier",
            "pitchMultiplier",
            "audioBank"          // Audio bank for sound samples

        };
        outputs = {
            "waveEmission",  // Generated acoustic waves
            "energy",        // Energy transfer data
            "resonance"      // Resonance effects
        };
    }
};

class AcousticSource : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AcousticSource(const AcousticSourceInfo& info = AcousticSourceInfo())
        : Node(info), AcousticSourceInfo(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    vec3 position;
    vec3 orientation;
    vec3 velocity;
    float volumeMultiplier = 1.0f;
    bool dynamicResponse = false;
    WavePattern wavePattern;
    MediumProperties medium;

    void addAudioBank(const std::string& audioBankPath);
    void removeAudioBank(const std::string& audioBankPath);

    // === Processing ===
    void processNode() override {
        position = getInputValue<vec3>("position");
        orientation = getInputValue<vec3>("orientation");
        velocity = getInputValue<vec3>("velocity");
        wavePattern = getInputValue<WavePattern>("wavePattern");
        medium = getInputValue<MediumProperties>("medium");
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AcousticSource() = default;     // Default destructor
};

} // namespace hd
