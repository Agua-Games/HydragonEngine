/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DisplaceDeformer.h
 * @brief Header file for the DisplaceDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - DisplaceDeformer is a class that represents a displacement deformer in Hydragon.
 * - It is used to displace a shape or object along a specified axis.
 * - It supports various displacement types, such as linear, circular, and helical.
 * - It also supports different displacement directions, such as positive and negative.
 * - It can be used to create various effects, such as bumps, ridges, and waves.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct DisplaceDeformerInfo : public DeformerInfo {
    DisplaceDeformerInfo() {
        nodeType = "Procedural/DisplaceDeformer";
        
        inputs = {
            "shape",                  // Shape to be displaced
            "displaceType",           // Linear, circular, helical, etc.
            "displaceAxis",           // Axis of displacement
            "displaceDirection",      // Positive or negative direction
            "displaceAmount",         // Amount of displacement
            "displaceCenter"          // Center of displacement
        };
        
        outputs = {
            "displacedShape",         // Displaced shape
            "displaceMetrics"         // Performance and quality metrics
        };
    }
};

class DisplaceDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit DisplaceDeformer(const DisplaceDeformerInfo& info = DisplaceDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DisplaceDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processDisplace();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DisplaceDeformer() = default;     // Default destructor
};

} // namespace hd

