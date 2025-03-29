/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RelaxDeformer.h
 * @brief Header file for the RelaxDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - RelaxDeformer is a class that represents a relaxation deformer in Hydragon.
 * - It is used to relax or smooth a shape or object along a specified axis.
 * - It supports various relaxation types, such as linear, circular, and helical.
 * - It also supports different relaxation directions, such as positive and negative.
 * - It can be used to create various effects, such as softening, blurring, and smoothing.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct RelaxDeformerInfo : public DeformerInfo {
    RelaxDeformerInfo() {
        nodeType = "Procedural/RelaxDeformer";
        
        inputs = {
            "shape",          // Shape to be relaxed or smoothed
            "relaxType",      // Linear, circular, helical, etc.
            "relaxAxis",      // Axis of relaxation or smoothing
            "relaxDirection", // Positive or negative direction
            "relaxAmount",    // Amount of relaxation or smoothing
            "relaxCenter"     // Center of relaxation or smoothing
        };
        
        outputs = {
            "relaxedShape",   // Relaxed or smoothed shape
            "relaxMetrics"    // Performance and quality metrics
        };
    }
};

class RelaxDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit RelaxDeformer(const RelaxDeformerInfo& info = RelaxDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    RelaxDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processRelax();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RelaxDeformer() = default;     // Default destructor
};

} // namespace hd

