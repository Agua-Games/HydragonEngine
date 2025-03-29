/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SquashDeformer.h
 * @brief Header file for the SquashDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - SquashDeformer is a class that represents a squash or stretch deformer in Hydragon.
 * - It is used to squash or stretch a shape or object along a specified axis.
 * - It supports various squash or stretch types, such as linear, circular, and helical.
 * - It also supports different squash or stretch directions, such as positive and negative.
 * - It can be used to create various effects, such as compression, flattening, and distortion.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct SquashDeformerInfo : public DeformerInfo {
    SquashDeformerInfo() {
        nodeType = "Procedural/SquashDeformer";
        
        inputs = {
            "shape",          // Shape to be squashed or stretched
            "squashType",     // Linear, circular, helical, etc.
            "squashAxis",     // Axis of squashing or stretching
            "squashDirection",// Positive or negative direction
            "squashAmount",   // Amount of squashing or stretching
            "squashCenter"    // Center of squashing or stretching
        };
        
        outputs = {
            "squashedShape",  // Squashed or stretched shape
            "squashMetrics"   // Performance and quality metrics
        };
    }
};

class SquashDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit SquashDeformer(const SquashDeformerInfo& info = SquashDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    SquashDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processSquash();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SquashDeformer() = default;     // Default destructor
};

} // namespace hd

