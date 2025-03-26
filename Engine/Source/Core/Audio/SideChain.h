/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SideChain.h
 * @brief Header file for the SideChain class.
 * 
 * ARCHITECTURAL NOTES:
 * - SideChain is a class that represents a sidechain effect in the engine's node graph.
 * - It is used to apply sidechain compression to audio signals.
 * - It uses the FMOD library for sidechain processing.
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

struct SideChainInfo : public NodeInfo {
    SideChainInfo() {
        nodeType = "Audio/SideChain";
        inputs = {
            "audioSignal",      // Audio signal to apply sidechain compression to
            "source",           // Audio signal to use as the sidechain source
            "amount",           // Amount of sidechain compression to apply
            "attack",           // Sidechain attack time
            "release",          // Sidechain release time
            "threshold",        // Sidechain threshold
            "ratio",            // Sidechain compression ratio
            "makeupGain",       // Makeup gain to apply after sidechain compression
            "randomize",        // Whether to randomize the sidechain compression
            "sideChainParams"   // Sidechain parameters
        };
        outputs = {
            "sideChainSignal",  // Sidechain-compressed audio signal
            "sideChainMetrics"  // Sidechain performance metrics
        };
    }
};

class SideChain : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SideChain(const SideChainInfo& info = SideChainInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    AudioSignal* source = nullptr;
    float amount = 0.0f;
    float attack = 0.0f;
    float release = 0.0f;
    float threshold = 0.0f;
    float ratio = 0.0f;
    float makeupGain = 0.0f;
    bool randomize = false;
    std::unordered_map<std::string, float> sideChainParams;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SideChain() = default;     // Default destructor
};

} // namespace hd
