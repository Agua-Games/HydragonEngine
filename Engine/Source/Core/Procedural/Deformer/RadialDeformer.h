/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RadialDeformer.h
 * @brief Header file for the RadialDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - RadialDeformer is a class that represents a radial deformer in Hydragon.
 * - It is used to deform a shape or object into a radial shape.
 * - It supports various radial types, such as circular, elliptical, and helical.
 * - It also supports different radial directions, such as positive and negative.
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

struct RadialDeformerInfo : public DeformerInfo {
    RadialDeformerInfo() {
        nodeType = "Procedural/RadialDeformer";
        
        inputs = {
            "shape",          // Shape to be deformed radially
            "radialType",     // Circular, elliptical, helical, etc.
            "radialAxis",     // Axis of radial deformation
            "radialDirection",// Positive or negative direction
            "radialAmount",   // Amount of radial deformation
            "radialCenter"    // Center of radial deformation
        };
        
        outputs = {
            "radialShape",    // Radially deformed shape
            "radialMetrics"   // Performance and quality metrics
        };
    }
};

class RadialDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit RadialDeformer(const RadialDeformerInfo& info = RadialDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    RadialDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processRadial();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RadialDeformer() = default;     // Default destructor
};

} // namespace hd

