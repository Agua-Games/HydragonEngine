/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Reverb.h
 * @brief Header file for the Reverb class.
 * 
 * ARCHITECTURAL NOTES:
 * - Reverb is a class that represents a reverb effect in the engine's node graph.
 * - It is used to apply reverb to audio signals.
 * - It uses the FMOD library for reverb processing.
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

struct ReverbInfo : public NodeInfo {
    ReverbInfo() {
        NodeType = "Audio/Reverb";
        inputs = {
            "AudioSignal",
            "RoomSize",  
            "Damping",  
            "WetLevel", 
            "DryLevel", 
            "ReverbParams" 
        };
        outputs = {
            "ReverbSignal",
            "ReverbMetrics"
        };
    }
};

class Reverb : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Reverb(const ReverbInfo& info = ReverbInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    float roomSize = 0.0f;
    float damping = 0.0f;
    float wetLevel = 0.0f;
    float dryLevel = 0.0f;

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Reverb() = default;     // Default destructor
};

} // namespace hd
