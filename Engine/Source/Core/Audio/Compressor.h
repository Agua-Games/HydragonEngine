/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Compressor.h
 * @brief Header file for the Compressor class.
 * 
 * ARCHITECTURAL NOTES:
 * - Compressor is a class that represents a compressor effect in the engine's node graph.
 * - It is used to compress audio signals.
 * - It uses the FMOD library for audio processing.
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

struct CompressorInfo : public NodeInfo {
    CompressorInfo() {
        NodeType = "Audio/Compressor";
        inputs = {
            "AudioSignal",   // Audio signal to compress
            "CompressorParams"  // Compressor parameters
        };
        outputs = {
            "CompressedSignal",  // Compressed audio signal
            "CompressorMetrics"  // Compressor performance metrics
        };
    }
};

class Compressor : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Compressor(const CompressorInfo& info = CompressorInfo())
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
    ~Compressor() = default;     // Default destructor
};

} // namespace hd
