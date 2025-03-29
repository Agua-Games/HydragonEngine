/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ArcDeformer.h
 * @brief Header file for the ArcDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - ArcDeformer is a class that represents an arc deformer in Hydragon.
 * - It is used to deform a shape or object into an arc shape.
 * - It supports various arc types, such as circular, elliptical, and helical.
 * - It also supports different arc directions, such as positive and negative.
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

struct ArcDeformerInfo : public DeformerInfo {
    ArcDeformerInfo() {
        nodeType = "Procedural/ArcDeformer";
        
        inputs = {
            "shape",          // Shape to be deformed into an arc
            "arcType",        // Circular, elliptical, helical, etc.
            "arcAxis",        // Axis of the arc
            "arcDirection",   // Positive or negative direction
            "arcRadius",      // Radius of the arc
            "arcCenter"       // Center of the arc
        };
        
        outputs = {
            "arcShape",       // Deformed arc shape
            "arcMetrics"      // Performance and quality metrics
        };
    }
};

class ArcDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit ArcDeformer(const ArcDeformerInfo& info = ArcDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ArcDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processArc();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ArcDeformer() = default;     // Default destructor
};

} // namespace hd

