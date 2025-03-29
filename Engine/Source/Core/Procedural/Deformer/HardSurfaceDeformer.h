/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HardSurfaceDeformer.h
 * @brief Header file for the HardSurfaceDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - HardSurfaceDeformer is a class that represents a hard surface deformer in Hydragon.
 * - It is used to create hard surface effects on a shape or object.
 * - It supports various hard surface types, such as linear, circular, and helical.
 * - It also supports different hard surface directions, such as positive and negative.
 * - It can be used to create various effects, such as ridges, grooves, and bumps.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct HardSurfaceDeformerInfo : public DeformerInfo {
    HardSurfaceDeformerInfo() {
        nodeType = "Procedural/HardSurfaceDeformer";
        
        inputs = {
            "shape",                  // Shape to be deformed into a hard surface
            "hardSurfaceType",        // Linear, circular, helical, etc.
            "hardSurfaceAxis",        // Axis of the hard surface
            "hardSurfaceDirection",   // Positive or negative direction
            "hardSurfaceAmount",      // Amount of hard surface deformation
            "hardSurfaceCenter"       // Center of the hard surface deformation
        };
        
        outputs = {
            "hardSurfaceShape",       // Deformed hard surface shape
            "hardSurfaceMetrics"      // Performance and quality metrics
        };
    }
};

class HardSurfaceDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit HardSurfaceDeformer(const HardSurfaceDeformerInfo& info = HardSurfaceDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    HardSurfaceDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processHardSurface();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~HardSurfaceDeformer() = default;     // Default destructor
};

}
