/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AnimationBlend.h
 * @brief Specialized animation blending node that extends the base Blend functionality.
 * 
 * ARCHITECTURAL NOTES:
 * - AnimationBlend provides optimized blending specifically for skeletal animations
 * - Supports quaternion interpolation, bone chain preservation, and volume preservation
 * - Integrates with the animation system for proper event handling and root motion
 * - Provides specialized optimizations for animation data using SIMD where available
 */
#pragma once
#include "Blend.h"
#include "Animation.h"
#include "SkeletonAnimation.h"
#include "Math/Quaternion.h"

namespace hd {

struct AnimationBlendInfo : public BlendInfo {
    AnimationBlendInfo() {
        nodeType = "Animation/AnimationBlend";
        
        inputs = {
            "sourceAnimation",      // First animation to blend
            "targetAnimation",      // Second animation to blend
            "blendFactor",         // Blend weight between animations
            "boneMask",           // Optional mask for partial skeleton blending
            "rootMotionMode",     // How to handle root motion during blend
            "timeWarpFactor"      // Optional time scaling between animations
        };
        
        outputs = {
            "blendedAnimation",    // Resulting blended animation
            "rootMotionDelta",    // Accumulated root motion
            "activeEvents",       // Animation events that should trigger
            "blendMetrics"       // Performance and quality metrics
        };
    }
};

/**
 * @class AnimationBlend
 * @brief Represents an animation blend node in the game world.
 */
class AnimationBlend : public Blend {
public:
    enum class RootMotionMode {
        Interpolate,    // Smoothly blend between both animations' root motion
        Source,         // Use only source animation's root motion
        Target,         // Use only target animation's root motion
        Disable        // Ignore root motion during blend
    };

    // === Allocation, Initialization, Loading ===
    explicit AnimationBlend(const AnimationBlendInfo& info = AnimationBlendInfo())
        : Blend(info) {}
    initialize() override {}
    load() override {}

    // Optimization Flags
    bool m_useSimpleBlend = true;           // Fast path for simple blending
    bool m_useRootMotion = false;           // Root motion handling
    bool m_useBoneMask = false;             // Partial skeleton blending
    bool m_useTimeWarp = false;             // Time scaling between animations

    // State
    RootMotionMode m_rootMotionMode = RootMotionMode::Interpolate;
    std::string m_activeBoneMask;
    float m_timeWarpSource = 1.0f;
    float m_timeWarpTarget = 1.0f;
    
    // Event handling
    std::unordered_map<std::string, std::function<void()>> m_eventCallbacks;

    // Set default values
    SkeletonAnimation* sourceAnimation = nullptr;
    SkeletonAnimation* targetAnimation = nullptr;
    float blendFactor = 0.0f;
    std::string boneMask;
    RootMotionMode rootMotionMode = RootMotionMode::Interpolate;
    float timeWarpFactor = 1.0f;

    // === Processing ===
    // Specialized Animation Blending API
    void addAnimation(SkeletonAnimation* anim);
    void setBoneMask(const std::string& maskName);
    void setRootMotionMode(RootMotionMode mode);
    void setTimeWarp(float sourceTime, float targetTime);
    
    // Event handling
    void addEventCallback(const std::string& eventName, std::function<void()> callback);
    void removeEventCallback(const std::string& eventName);

    void processNode() override {
        if (m_useSimpleBlend) {
            processSimpleAnimationBlend();  // Fast path for simple cases
        } else {
            processComplexAnimationBlend(); // Full-featured path
        }
    }

    void processSimpleAnimationBlend() {
        sourceAnimation = getInputValue<SkeletonAnimation>("sourceAnimation");
        targetAnimation = getInputValue<SkeletonAnimation>("targetAnimation");
        blendFactor = getInputValue<float>("blendFactor");
        boneMask = getInputValue<std::string>("boneMask");
        rootMotionMode = getInputValue<RootMotionMode>("rootMotionMode");
        timeWarpFactor = getInputValue<float>("timeWarpFactor");
        
        // Fast path: direct quaternion interpolation and position blending
        auto blendedPose = blendSkeletonPoses(sourceAnimation->getCurrentPose(), 
                                             targetAnimation->getCurrentPose(), 
                                             blendFactor);  // Use the input value for blendFactor
        
        setOutputValue("blendedAnimation", blendedPose);
        setOutputValue("rootMotionDelta", calculateRootMotionDelta(sourceAnimation, targetAnimation, blendFactor));  // Use the input value for blendFactor
        setOutputValue("activeEvents", getActiveEvents());
        setOutputValue("blendMetrics", getBlendMetrics());
    }

    void processComplexAnimationBlend() {
        // Handle advanced features: bone masks, root motion, time warping, etc.
        processPartialSkeletonBlend();
        processRootMotion();
        processAnimationEvents();
    }

    // Specialized Blending Functions
    Pose blendSkeletonPoses(const Pose& source, const Pose& target, float factor);
    void processPartialSkeletonBlend();
    void processRootMotion();
    void processAnimationEvents();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AnimationBlend() = default;     // Default destructor
};


