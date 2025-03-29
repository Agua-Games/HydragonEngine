/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WaveDeformer.h
 * @brief Header file for the WaveDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - WaveDeformer is a class that represents a wave deformer in Hydragon.
 * - It is used to deform a shape or object into a wave shape.
 * - It supports various wave types, such as sine, cosine, and helical.
 * - It also supports different wave directions, such as positive and negative.
 * - It can be used to create various effects, such as ripples, waves, and undulations.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct WaveDeformerInfo : public DeformerInfo {
    WaveDeformerInfo() {
        nodeType = "Procedural/WaveDeformer";
        
        inputs = {
            "shape",          // Shape to be deformed into a wave
            "waveType",       // Sine, cosine, helical, etc.
            "waveAxis",       // Axis of the wave
            "waveDirection",  // Positive or negative direction
            "waveAmplitude",  // Amplitude of the wave
            "waveFrequency",  // Frequency of the wave
            "wavePhase"       // Phase of the wave
        };
        
        outputs = {
            "waveShape",      // Deformed wave shape
            "waveMetrics"     // Performance and quality metrics
        };
    }
};

class WaveDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit WaveDeformer(const WaveDeformerInfo& info = WaveDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    WaveDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processWave();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WaveDeformer() = default;     // Default destructor
};

} // namespace hd

