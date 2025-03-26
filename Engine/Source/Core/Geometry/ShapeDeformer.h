/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ShapeDeformer.h
 * @brief Header file for the ShapeDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - ShapeDeformer is a class that represents a shape deformer in Hydragon.
 * - It is used to deform a shape based on a set of control points or vertices and weights. It can be used to create morph targets or blend shapes.
 * - It offers built-in support for procedural modification of imported shapes, and for procedural shape generation.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Deformer.h"

namespace hd {

struct ShapeDeformerInfo : public DeformerInfo {
    ShapeDeformerInfo() {
        NodeType = "Geometry/ShapeDeformer";
        
        inputs = {
            "ControlPoints",   // Array of control points
            "Weights",         // Corresponding weights
            "DeformationMode", // Linear, cubic, spline, etc.
            "EasingFunction",  // Ease-in, ease-out, etc.
            "Time",            // Current time
            "PlaybackSpeed",   // Playback speed
            "Looping"          // Whether to loop the deformation
        };
        
        outputs = {
            "DeformedShape",   // Deformed shape data
            "DeformMetrics"    // Performance and quality metrics
        };
    }
};

class ShapeDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit ShapeDeformer(const ShapeDeformerInfo& info = ShapeDeformerInfo())
        : Deformer(info) {}
    initialize() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ShapeDeformer() = default;     // Default destructor
};

} // namespace hd
