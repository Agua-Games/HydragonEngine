/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BendDeformer.h
 * @brief Header file for the BendDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - BendDeformer is a class that represents a bend deformer in Hydragon.
 * - It is used to bend a shape or object along a specified axis.
 * - It supports various bend types, such as linear, circular, and helical.
 * - It also supports different bend directions, such as positive and negative.
 * - It can be used to create various effects, such as arches, curves, and spirals.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct BendDeformerInfo : public DeformerInfo {
    BendDeformerInfo() {
        nodeType = "Procedural/BendDeformer";
        
        inputs = {
            "shape",          // Shape to be bent
            "bendType",       // Linear, circular, helical, etc.
            "bendAxis",       // Axis of bending
            "bendDirection",  // Positive or negative direction
            "bendAmount",     // Amount of bending
            "bendCenter"      // Center of bending
        };
        
        outputs = {
            "bentShape",      // Bent shape
            "bendMetrics"     // Performance and quality metrics
        };
    }
};

class BendDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit BendDeformer(const BendDeformerInfo& info = BendDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    BendDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processBend();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BendDeformer() = default;     // Default destructor
};

} // namespace hd
