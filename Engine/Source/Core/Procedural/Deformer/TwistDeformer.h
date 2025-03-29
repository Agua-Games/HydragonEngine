/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TwistDeformer.h
 * @brief Header file for the TwistDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - TwistDeformer is a class that represents a twist deformer in Hydragon.
 * - It is used to twist a shape or object along a specified axis.
 * - It supports various twist types, such as linear, circular, and helical.
 * - It also supports different twist directions, such as positive and negative.
 * - It can be used to create various effects, such as spirals, helices, and vortices.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct TwistDeformerInfo : public DeformerInfo {
    TwistDeformerInfo() {
        nodeType = "Procedural/TwistDeformer";
        
        inputs = {
            "shape",          // Shape to be twisted
            "twistType",      // Linear, circular, helical, etc.
            "twistAxis",      // Axis of twisting
            "twistDirection", // Positive or negative direction
            "twistAmount",    // Amount of twisting
            "twistCenter"     // Center of twisting
        };
        
        outputs = {
            "twistedShape",   // Twisted shape
            "twistMetrics"    // Performance and quality metrics
        };
    }
};

class TwistDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit TwistDeformer(const TwistDeformerInfo& info = TwistDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    TwistDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {    
    
    }
    void processTwist();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TwistDeformer() = default;     // Default destructor
};

} // namespace hd
