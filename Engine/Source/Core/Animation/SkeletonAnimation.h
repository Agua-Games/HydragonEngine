/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SkeletonAnimation.h
 * @brief Header file for the SkeletonAnimation class.
 * 
 * ARCHITECTURAL NOTES:
 * - SkeletonAnimation is a class that represents a skeleton animation in Hydragon.
 * - It is used to animate a skeleton and its associated geometry.
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

struct SkeletonAnimationInfo : public AnimationInfo {
    SkeletonAnimationInfo() {
        NodeType = "Animation/SkeletonAnimation";
        
        inputs = {
            "Skeleton",        // Skeleton data
            "AnimationClip",   // Animation clip data
            "Time",            // Current time
            "PlaybackSpeed",   // Playback speed
            "Looping"          // Whether to loop the animation
        };
        
        outputs = {
            "AnimatedSkeleton",  // Animated skeleton data
            "AnimationMetrics"   // Performance and quality metrics
        };
    }
};

class SkeletonAnimation : public Animation {
public:
    // === Allocation, Initialization, Loading ===
    explicit SkeletonAnimation(const SkeletonAnimationInfo& info = SkeletonAnimationInfo())
        : Node(info) {}
    void initialize();

    // === Processing ===
    void processAnimation();                    // Process the animation. This is called every frame.
    void applyAnimationToSkeleton();           // Apply the animation to the skeleton. This is called every frame.
    void processNodeGraph() override {          // Process the node graph. This is called every frame.
        update();
    }     
    void update();                              // Update the animation state. This is called every frame.

    // === Cleanup ===
    ~SkeletonAnimation() = default;
};

} // namespace hd