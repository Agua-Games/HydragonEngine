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
        NodeType = "Audio/SideChain";
        inputs = {
            "AudioSignal",   // Audio signal to apply sidechain compression to
            "SideChainParams"  // Sidechain parameters
        };
        outputs = {
            "SideChainSignal",  // Sidechain-compressed audio signal
            "SideChainMetrics"  // Sidechain performance metrics
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

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SideChain() = default;     // Default destructor
};

} // namespace hd
