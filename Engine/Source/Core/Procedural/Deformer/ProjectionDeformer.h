/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProjectionDeformer.h
 * @brief Header file for the ProjectionDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - ProjectionDeformer is a class that represents a projection to surface deformer in Hydragon.
 * - It is used to project a shape or object onto a surface.
 * - It supports various projection types, such as linear, circular, and helical.
 * - It also supports different projection directions, such as positive and negative.
 * - It can be used to create various effects, such as embossing, debossing, and extrusion.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct ProjectionDeformerInfo : public DeformerInfo {
    ProjectionDeformerInfo() {
        nodeType = "Procedural/ProjectionDeformer";
        
        inputs = {
            "shape",                  // Shape to be projected onto a surface
            "projectionType",         // Linear, circular, helical, etc.
            "projectionAxis",         // Axis of projection
            "projectionDirection",    // Positive or negative direction
            "projectionAmount",       // Amount of projection
            "projectionCenter"        // Center of projection
        };
        
        outputs = {
            "projectedShape",         // Projected shape
            "projectionMetrics"       // Performance and quality metrics
        };
    }
};

class ProjectionDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit ProjectionDeformer(const ProjectionDeformerInfo& info = ProjectionDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ProjectionDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processProjection();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ProjectionDeformer() = default;     // Default destructor
};

} // namespace hd
