/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AcousticProcessor.h
 * @brief Header file for the AcousticProcessor class.
 * 
 * ARCHITECTURAL NOTES:
 * - AcousticProcessor is a class that processes acoustic waves and applies acoustic effects based on the medium properties.
 * - It provides methods to process the wave interaction and apply the acoustic effects.
 */
#pragma once
#include <fmod.hpp>
#include <string>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "PhysicsTypes.h"
#include "AcousticSource.h"
#include "AcousticListener.h"
#include "AudioManager.h"

namespace hd {

struct AcousticProcessorInfo : public NodeInfo {
    AcousticProcessorInfo() {
        nodeType = "Audio/AcousticProcessor";
        inputs = {
            "waveInteraction",  // Wave interaction data
            "mediumProperties" // Medium properties
        };
        outputs = {
            "processedWave",    // Processed acoustic wave
            "acousticEffects"   // Applied acoustic effects
        };
    }
};

class AcousticProcessor : public Node {
public:
    // === Allocation, Initialization, Loading ===
    AcousticProcessor() = default;
    ~AcousticProcessor() = default;
    initialize() override {}

    // Set default values
    AcousticWaveInteraction waveInteraction;
    MediumProperties mediumProperties;

    // === Processing ===
    void processNode() override {
        waveInteraction = getInputValue<AcousticWaveInteraction>("waveInteraction");
        mediumProperties = getInputValue<MediumProperties>("mediumProperties");
    }
    void update();
    AcousticModulation process(const AcousticWaveInteraction& interaction, const MediumProperties& medium);
    void applyAcousticEffects(const AcousticWave& wave, const AcousticModulation& modulation);

    // === Cleanup ===
    void cleanup() override {}
};

} // namespace hd

