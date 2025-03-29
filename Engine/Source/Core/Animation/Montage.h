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
 * 
 * @todo Centralize the types (structs, enums) in common between all nodes in their proper header files (e.g. AnimationTypes.h), then replace them, call the proper includes.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"
#include "SkeletonDeformer.h"

namespace hd {

struct MontageInfo : public AnimationInfo {
    MontageInfo() {
        nodeType = "Animation/Montage";
        
        inputs = {
            "montage",         // Montage data
            "skeleton",        // Skeleton data
            "animationClip",   // Animation clip data
            "time",            // Current time
            "playbackSpeed",   // Playback speed
            "looping"          // Whether to loop the animation
        };
        
        outputs = {
            "animatedMontage", // Animated montage data
            "animationMetrics" // Performance and quality metrics
        };
    }
};

class Montage : public Animation {
public:
    enum class InterpolationMode {
        Linear,
        Cubic,
        Spline
    };

    enum class EasingFunction {
        None,
        EaseIn,
        EaseOut,
        EaseInOut
    };

    enum class Mode {
        Basic,
        Advanced
    };

    // === Allocation, Initialization, Loading ===
    explicit Montage(const MontageInfo& info = MontageInfo())
        : Animation(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    Mode mode = Mode::Basic;
    InterpolationMode interpolationMode = InterpolationMode::Linear;
    EasingFunction easingFunction = EasingFunction::None;
    float length = 0.0f;
    bool enableCache = false;

    // === Processing ===
    void processNode() override {
        auto montage = getInputValue<Montage>("Montage");
        auto skeleton = getInputValue<Skeleton>("Skeleton");
        auto animationClip = getInputValue<AnimationClip>("AnimationClip");
        auto time = getInputValue<float>("Time");
        auto playbackSpeed = getInputValue<float>("PlaybackSpeed");
        auto looping = getInputValue<bool>("Looping");
        
        auto animatedMontage = animateMontage(montage, skeleton, animationClip, time, playbackSpeed, looping);
        auto metrics = analyzeAnimationQuality(animatedMontage);
    }
    void cacheTrack(const std::string& trackName);
    void cacheMontage(const Montage& montage);
    void clearCache();
    void animateMontage(const Montage& montage, const Skeleton& skeleton, const AnimationClip& animationClip, float time, float playbackSpeed, bool looping);
    void stopMontage();
    void playMontage();
    void pauseMontage();
    void resumeMontage();
    void rewindMontage();
    void fastForwardMontage();
    void advanceMontage();                  // Advance the montage by one frame
    void stepBackMontage();                 // Step the montage by one frame
    void analyzeAnimationQuality(const Montage& animatedMontage);
    void linkObject(const std::string& objectName);                                      // Link the object to animate to a track
    void unlinkObject(const std::string& objectName);                                   // Unlink the object's link with the track, keeping the track and its keyframes
    void swapObjects(const std::string& objectName1, const std::string& objectName2);   // Swap the objects' links with the track
    void addTrack(const std::string& trackName);
    void removeTrack(const std::string& trackName);
    void addClip(const std::string& clipName, const std::string& trackName);
    void removeClip(const std::string& clipName, const std::string& trackName);
    void addLayer(const std::string& layerName);
    void removeLayer(const std::string& layerName);
    void addKeyframe(const std::string& trackName, float time, const std::string& value);
    void removeKeyframe(const std::string& trackName, float time);
    void setKeyframeValue(const std::string& trackName, float time, const std::string& value);
    void setKeyframeTime(const std::string& trackName, float oldTime, float newTime);
    void blendAnimations(const std::string& trackName);
    void blendAnimationWithPhysics(const std::string& trackName);       // To set physics properties we link the node we want to a track
    void setInterpolationMode(const std::string& mode);
    void setEasingFunction(const std::string& function);    // Set the easing function for the track, like ease-in, ease-out, ease-in-out, etc.

    void setProperty(const std::string& propertyName, const std::string& propertyValue);
    void getProperty(const std::string& propertyName);
    void setMode(const std::string& mode);      // Toggle between basic and advanced modes
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Montage() = default;     // Default destructor
};

} // namespace hd
