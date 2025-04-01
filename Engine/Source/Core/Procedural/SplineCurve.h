/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SplineCurve.h
 * @brief Header file for the SplineCurve class.
 * 
 * ARCHITECTURAL NOTES:
 * - SplineCurve is a class that represents a spatial curve (a SP-line) in Hydragon.
 * - It is used to represent and process spatial curves, such as paths, splines, etc.
 * - It can be used along with other nodes to represent, for instance, a path for a character to follow, swarms of fishes which follow a path, effects which follow a path, etc.
 * - It supports built-in support for procedural modification of imported curves, and for procedural curve generation, leveraging noise, patterns, etc.
 * - It supports built-in physics simulation and integration with WavePhysics, for effects like gravity, wind, springs, turbulence, etc.
 * - It supports physics fields, being able to influence the vectors in the field with controls for amount of influence, etc. It also supports physics fields for the tangents.
 * - It uses the Vulkan API for spatial curve processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "DataCurve.h"

namespace hd {

struct SplineCurveInfo : public DataCurveInfo {
    SplineCurveInfo() {
        NodeType = "Procedural/SplineCurve";
        inputs = {
            "ControlPoints",  // Control points of the curve
            "Tangents",       // Tangents at the control points
            "Interpolation",  // Interpolation mode
            "Easing",         // Easing function
            "Looping"         // Whether to loop the curve
        };
        outputs = {
            "Curve",          // Curve data
            "Metrics"         // Performance and quality metrics
        };
    }
};

class SplineCurve : public DataCurve {
public:
    // === Allocation, Initialization, Loading === 
    explicit SplineCurve(const SplineCurveInfo& info = SplineCurveInfo())
        : DataCurve(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    }
    void processCurve();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SplineCurve() = default;     // Default destructor
};

} // namespace hd
