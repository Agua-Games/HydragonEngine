/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ClothDeformer.h
 * @brief Header file for the ClothDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - ClothDeformer is a class that represents a cloth simulation deformer in Hydragon.
 * - It is used to simulate the behavior of cloth or fabric on a shape or object.
 * - It supports various cloth simulation types, such as linear, circular, and helical.
 * - It also supports different cloth simulation directions, such as positive and negative.
 * - It can be used to create various effects, such as draping, folding, and waving.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct ClothDeformerInfo : public DeformerInfo {
    ClothDeformerInfo() {
        nodeType = "Procedural/ClothDeformer";
        
        inputs = {
            "shape",          // Shape to be cloth-simulated
            "clothType",      // Linear, circular, helical, etc.
            "clothAxis",      // Axis of cloth simulation
            "clothDirection", // Positive or negative direction
            "clothAmount",    // Amount of cloth simulation
            "clothCenter"     // Center of cloth simulation
        };
        
        outputs = {
            "clothShape",     // Cloth-simulated shape
            "clothMetrics"    // Performance and quality metrics
        };
    }
};

class ClothDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit ClothDeformer(const ClothDeformerInfo& info = ClothDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ClothDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processCloth();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ClothDeformer() = default;     // Default destructor
};

} // namespace hd

