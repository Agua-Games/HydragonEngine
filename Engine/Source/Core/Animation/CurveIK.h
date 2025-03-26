/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CurveIK.h
 * @brief Header file for the CurveIK class.
 * 
 * ARCHITECTURAL NOTES:
 * - CurveIK is a class that represents a curve inverse kinematics in Hydragon.
 * - It is used to animate a curve and its associated geometry, joints or any other arbitrary data.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It uses the Vulkan API for animation processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"
#include "SkeletonDeformer.h"

namespace hd {

struct CurveIKInfo : public AnimationInfo {
    CurveIKInfo() {
        nodeType = "nimation/CurveIK";
        
        inputs = {
            "curve",           // Curve data
            "animationClip",   // Animation clip data
            "time",            // Current time
            "playbackSpeed",   // Playback speed
            "looping"          // Whether to loop the animation
        };
        
        outputs = {
            "animatedCurve",   // Animated curve data
            "animationMetrics" // Performance and quality metrics
        };
    }
};

class CurveIK : public Animation {
public:
    // === Allocation, Initialization, Loading ===
    explicit CurveIK(const CurveIKInfo& info = CurveIKInfo())
        : Animation(info) {}
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
    ~CurveIK() = default;     // Default destructor
};

} // namespace hd
