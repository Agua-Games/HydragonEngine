/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Montage.h
 * @brief Header file for the Montage class.
 * 
 * ARCHITECTURAL NOTES:
 * - Montage is a class that represents a montage in Hydragon.
 * - It is used to animate a montage and its associated geometry, joints or any other arbitrary data.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It has native support for blending and overriding WavePhysics and has physics properties and features, like elasticEnergy/spring, damping, tension, bending, etc.
 * - It supports different interpolation modes, such as linear, cubic, and spline interpolation.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It has two basic modes: basic and advanced:
 *      - Basic mode is basically a timeline for simple animations
 *      - Advanced mode is for complex animations: it supports multiple tracks, animation clip blending, blending animations, blending animation with
 * physics, and more.
 * - It uses the Vulkan API for animation processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"
#include "SkeletonDeformer.h"

namespace hd {

struct MontageInfo : public AnimationInfo {
    MontageInfo() {
        NodeType = "Animation/Montage";
        
        inputs = {
            "Montage",         // Montage data
            "Skeleton",        // Skeleton data
            "AnimationClip",   // Animation clip data
            "Time",            // Current time
            "PlaybackSpeed",   // Playback speed
            "Looping"          // Whether to loop the animation
        };
        
        outputs = {
            "AnimatedMontage", // Animated montage data
            "AnimationMetrics" // Performance and quality metrics
        };
    }
};

class Montage : public Animation {
public:
    // === Allocation, Initialization, Loading ===
    explicit Montage(const MontageInfo& info = MontageInfo())
        : Animation(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        auto montage = getInputValue<Montage>("Montage");
        auto skeleton = getInputValue<Skeleton>("Skeleton");
        auto animationClip = getInputValue<AnimationClip>("AnimationClip");
        auto time = getInputValue<float>("Time");
        auto playbackSpeed = getInputValue<float>("PlaybackSpeed");
        auto looping = getInputValue<bool>("Looping");
        
        auto animatedMontage = animateMontage(montage, skeleton, animationClip, time, playbackSpeed, looping);
        auto metrics = analyzeAnimationQuality(animatedMontage);
    }     // processNodeGraph() override

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Montage() = default;     // Default destructor
};

} // namespace hd
