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
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Node.h"

namespace hd {

struct AnimationInfo : public NodeInfo {
    AnimationInfo() {
        NodeType = "Animation/Animation";
        
        inputs = {
            "StartTime",       // Start time of the animation
            "EndTime",         // End time of the animation
            "Value",           // Value to animate
            "Interpolation",   // Interpolation mode
            "Easing",          // Easing function
            "Looping"          // Whether to loop the animation
        };
        
        outputs = {
            "AnimatedValue",   // Animated value
            "AnimationMetrics" // Performance and quality metrics
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

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Animation() = default;     // Default destructor
};

} // namespace hd
