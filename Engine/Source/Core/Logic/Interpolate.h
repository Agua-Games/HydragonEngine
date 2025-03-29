/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Interpolate.h
 * @brief Header file for the Interpolate class.
 * 
 * ARCHITECTURAL NOTES:
 * - Interpolate is a class that represents an interpolation in Hydragon.
 * - It is used to interpolate between two values.
 * - It supports different interpolation modes, such as linear, cubic, spline, and custom interpolation.
 * - Custom interpolation is supported by using a custom function.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It offers built-in support for procedural modification of imported interpolations, and for procedural interpolation generation.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Animation.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

enum class InterpolationMode {
    Linear,
    Cubic,
    Spline,
    Custom
};

struct InterpolateInfo : public NodeInfo {
    InterpolateInfo() {
        NodeType = "Logic/Interpolate";
        
        inputs = {
            "StartValue",  // Start value
            "EndValue",    // End value
            "Interpolation", // Interpolation mode
            "Easing",        // Easing function
            "Looping"         // Whether to loop the interpolation
        };
        
        outputs = {
            "InterpolatedValue" // Interpolated value
        };
    }
};

class Interpolate : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Interpolate(const InterpolateInfo& info = InterpolateInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    InterpolateInfo info;
    InterpolationMode mode = InterpolationMode::Linear;
    Animation animation;
    DataTable dataTable;
    
    // === Processing ===
    void processNode() override {
 
    }
    void processInterpolate();     // Process interpolation logic
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Interpolate() = default;     // Default destructor
};

} // namespace hd
