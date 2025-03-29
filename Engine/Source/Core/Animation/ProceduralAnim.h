/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralAnim.h
 * @brief Header file for the ProceduralAnim class.
 * 
 * ARCHITECTURAL NOTES:
 * - ProceduralAnim is a class that represents a procedural animation in Hydragon.
 * - It is used to animate any value with support for time-varying interpolation, such as position, rotation, scale, color, etc. 
 * - It can also be used to animate any other type of data.
 * - It can be blended with keyframed animations, and the blend value can be procedurally generated or keyframed.
 * - It supports built-in interpolated noise types, such as Perlin, Simplex, and Worley, and patterns.
 * - It supports different interpolation modes, such as linear, cubic, and spline interpolation.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It uses the Vulkan API for animation processing.
 */
#pragma once
#include "Animation.h"
#include "ProceduralTypes.h"

namespace hd {

struct ProceduralAnimInfo : public AnimationInfo {
    ProceduralAnimInfo() {
        nodeType = "Animation/ProceduralAnim";
        
        inputs = {
            "value",           // Value to animate
            "duration",        // Duration of the animation
            "tickRate",        // Tick rate for procedural animation
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

class ProceduralAnim : public Animation {
public:
    // === Allocation, Initialization, Loading ===
    explicit ProceduralAnim(const ProceduralAnimInfo& info = ProceduralAnimInfo())
        : Animation(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    NoiseType noiseType = NoiseType::Perlin;
    ProceduralStructureType structureType = ProceduralStructureType::Organic;
    uint32_t seed = 0;
    float scale = 1.0f;
    float complexity = 0.5f;
    float variation = 0.5f;
    float frequency = 1.0f;
    float evolution = 0.0f;
    bool looping = false;
    float value = 0.0f;
    float duration = 0.0f;
    float tickRate = 0.0f;
    InterpolationMode interpolation = InterpolationMode::Linear;
    EasingFunction easing = EasingFunction::None;

    // === Processing ===
    void processNode() override { }
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ProceduralAnim() = default;     // Default destructor
};

} // namespace hd
