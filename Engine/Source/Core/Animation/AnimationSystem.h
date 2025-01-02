/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Animation system for skeletal and property animation
 */

#pragma once
#include "AnimationTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Animation {

class AnimationSystem {
public:
    struct AnimationConfig {
        uint32_t maxAnimations = 1000;
        uint32_t maxSkeletons = 500;
        uint32_t maxBones = 10000;
        bool enableGPUSkinning = true;
        bool enableCompression = true;
        bool enableBlending = true;
        size_t memoryBudget = 128 * 1024 * 1024;  // 128MB
    };

    static AnimationSystem& Get();
    
    void Initialize(const AnimationConfig& config = {});
    void Shutdown();
    
    void Update(float deltaTime);
    
    AnimationHandle CreateAnimation(const AnimationDesc& desc);
    void DestroyAnimation(AnimationHandle handle);
    
    SkeletonHandle CreateSkeleton(const SkeletonDesc& desc);
    void DestroySkeleton(SkeletonHandle handle);
    
    void PlayAnimation(AnimationHandle anim, SkeletonHandle skeleton);
    void StopAnimation(AnimationHandle anim, SkeletonHandle skeleton);
    void BlendAnimation(AnimationHandle from, AnimationHandle to, float blendTime);
    
    void SetAnimationTime(AnimationHandle handle, float time);
    void SetAnimationSpeed(AnimationHandle handle, float speed);
    
    const AnimationStats& GetStats() const { return m_Stats; }

private:
    AnimationSystem() = default;
    
    AnimationConfig m_Config;
    AnimationStats m_Stats;
    std::unique_ptr<IAnimationProcessor> m_Processor;
    std::vector<Animation> m_Animations;
    std::vector<Skeleton> m_Skeletons;
    bool m_Initialized = false;
};

} // namespace Hydragon::Animation 