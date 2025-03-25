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
#include <fmod.hpp>
#include <string>
#include "Node.h"

namespace hd {

struct AcousticSourceInfo : public NodeInfo {
    AcousticSourceInfo() {
        NodeType = "Audio/AcousticSource";
        inputs = {
            "Position",      // Wave emission point
            "Orientation",   // Emission direction
            "Velocity",      // For Doppler effect
            "WavePattern",   // Acoustic wave characteristics
            "Medium"         // Propagation medium properties
        };
        outputs = {
            "WaveEmission",  // Generated acoustic waves
            "Energy",        // Energy transfer data
            "Resonance"      // Resonance effects
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

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AcousticSource() = default;     // Default destructor
};

} // namespace hd
