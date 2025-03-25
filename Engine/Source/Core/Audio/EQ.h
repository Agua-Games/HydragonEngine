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
        NodeType = "Audio/EQ";
        inputs = {
            "AudioSignal",   // Audio signal to equalize
            "EQParams"  // Equalizer parameters
        };
        outputs = {
            "EqualizedSignal",  // Equalized audio signal
            "EQMetrics"  // Equalizer performance metrics
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

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EQ() = default;     // Default destructor
};

} // namespace hd
