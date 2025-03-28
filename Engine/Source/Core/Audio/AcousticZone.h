/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AcousticZone.h
 * @brief Header file for the AcousticZone class.
 * 
 * ARCHITECTURAL NOTES:
 * - AcousticZone is a node class that represents a zone/volume with acoustic properties.
 * - It supports automatic placement of AcousticSources, and calculation of acoustic wave reflection (echoing), refraction, reverberation, etc.
 * - It supports different interpolation modes, such as linear, cubic, and spline interpolation.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It offers built-in support for procedural modification of imported nodes, and for procedural node generation.
 */
#pragma once
#include "Node.h"
#include "AcousticSource.h"
#include "AcousticListener.h"
#include "AcousticProcessor.h"

namespace hd {

struct AcousticZoneInfo : public NodeInfo {
    AcousticZoneInfo() {
        nodeType = "Audio/AcousticZone";
        inputs = {
            "position",          // Zone position
            "size",              // Zone size
            "shape",             // Zone shape
            "medium",            // Propagation medium properties
            "acousticParams"     // Acoustic parameters
        };
        outputs = {
            "acousticWave",  // Generated acoustic waves
            "energy",        // Energy transfer data
            "resonance"      // Resonance effects
        };
    }
};

class AcousticZone : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AcousticZone(const AcousticZoneInfo& info = AcousticZoneInfo())
        : Node(info), AcousticZoneInfo(info) {}
    initialize() override {}
    load() override {}
    unload() override {}

    // Set default values
    bool echoes = true;
    bool reverberation = true;
    float darknessBias = 0.0f;

    // === Processing ===
    void processNode() override {}

    // === Cleanup ===
    void cleanup() override {}
    ~AcousticZone() = default;     // Default destructor
};

} // namespace hd
