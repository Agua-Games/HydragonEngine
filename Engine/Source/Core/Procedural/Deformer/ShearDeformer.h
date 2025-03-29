/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ShearDeformer.h
 * @brief Header file for the ShearDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - ShearDeformer is a class that represents a shear deformer in Hydragon.
 * - It is used to shear a shape or object along a specified axis.
 * - It supports various shear types, such as linear, circular, and helical.
 * - It also supports different shear directions, such as positive and negative.
 * - It can be used to create various effects, such as slanting, tilting, and skewing.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct ShearDeformerInfo : public DeformerInfo {
    ShearDeformerInfo() {
        nodeType = "Procedural/ShearDeformer";
        
        inputs = {
            "shape",          // Shape to be sheared
            "shearType",      // Linear, circular, helical, etc.
            "shearAxis",      // Axis of shearing
            "shearDirection", // Positive or negative direction
            "shearAmount",    // Amount of shearing
            "shearCenter"     // Center of shearing
        };
        
        outputs = {
            "shearedShape",   // Sheared shape
            "shearMetrics"    // Performance and quality metrics
        };
    }
};

class ShearDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit ShearDeformer(const ShearDeformerInfo& info = ShearDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ShearDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processShear();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ShearDeformer() = default;     // Default destructor
};

} // namespace hd

