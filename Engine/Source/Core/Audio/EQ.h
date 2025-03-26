/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EQ.h
 * @brief Header file for the EQ class.
 * 
 * ARCHITECTURAL NOTES:
 * - EQ is a class that represents an equalizer effect in the engine's node graph.
 * - It is used to apply equalization to audio signals.
 * - It uses the FMOD library for equalization processing.
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

struct EQInfo : public NodeInfo {
    EQInfo() {
        nodeType = "Audio/EQ";
        inputs = {
            "audioSignal",
            "lowcut",   
            "highcut", 
            "bandParams", 
            "gain",
            "qFactor",      // Quality factor
            "eqParams"
        };
        outputs = {
            "EqualizedSignal",
            "EQMetrics"
        };
    }
};

class EQ : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit EQ(const EQInfo& info = EQInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    float lowcut = 0.0f;
    float highcut = 0.0f;
    std::unordered_map<float, std::pair<float, float>> bandParams;
    float gain = 0.0f;
    float qFactor = 0.0f;
    std::unordered_map<std::string, float> eqParams;

    // === Processing ===
    void addBand(float frequency, float gain, float qFactor);
    void removeBand(float frequency);
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EQ() = default;     // Default destructor
};

} // namespace hd
