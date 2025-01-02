/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Animation system type definitions
 */

#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Quaternion.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Animation {

using AnimationHandle = uint32_t;
using SkeletonHandle = uint32_t;
using BoneHandle = uint32_t;

enum class AnimationType {
    Skeletal,
    Morph,
    Property
};

enum class InterpolationType {
    Linear,
    Cubic,
    Step
};

struct KeyFrame {
    float time;
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    InterpolationType interpolation;
};

struct AnimationTrack {
    BoneHandle targetBone;
    std::vector<KeyFrame> keyframes;
    bool enabled = true;
    float weight = 1.0f;
};

struct AnimationDesc {
    std::string name;
    AnimationType type;
    float duration;
    bool looping = false;
    std::vector<AnimationTrack> tracks;
};

struct BoneDesc {
    std::string name;
    BoneHandle parentBone;
    Matrix4 bindPose;
    Vector3 bindPosition;
    Quaternion bindRotation;
};

struct SkeletonDesc {
    std::string name;
    std::vector<BoneDesc> bones;
    uint32_t maxInfluences = 4;
};

struct AnimationStats {
    uint32_t activeAnimations;
    uint32_t activeSkeletons;
    uint32_t totalBones;
    uint32_t blendOperations;
    float processingTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Animation 