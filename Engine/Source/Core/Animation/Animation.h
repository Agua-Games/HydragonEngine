/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Animation.h
 * @brief Header file for the Animation class.
 * 
 * ARCHITECTURAL NOTES:
 * - Animation is a class that represents an animation in Hydragon.
 * - It is used to animate any value with support for time-varying interpolation, such as position, rotation, scale, color, etc. 
 * - It can also be used to animate any other type of data.
 * - It supports different interpolation modes, such as linear, cubic, and spline interpolation.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It uses the Vulkan API for animation processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Node.h"

namespace hd {

struct AnimationInfo : public NodeInfo {
    AnimationInfo() {
        nodeType = "Animation/Animation";
        
        inputs = {
            "startTime",       // Start time of the animation
            "endTime",         // End time of the animation
            "value",           // Value to animate
            "interpolation",   // Interpolation mode
            "easing",          // Easing function
            "looping"          // Whether to loop the animation
        };
        
        outputs = {
            "animatedValue",   // Animated value
            "animationMetrics" // Performance and quality metrics
        };
    }
};

class Animation : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Animation(const AnimationInfo& info = AnimationInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)
    
    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Animation() = default;     // Default destructor
};

} // namespace hd
