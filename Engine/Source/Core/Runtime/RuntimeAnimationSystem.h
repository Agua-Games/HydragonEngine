/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime animation and skeletal management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeAnimationConfig {
    bool enableBlending = true;
    bool enableIK = true;
    bool enableRagdoll = true;
    bool enableMorphTargets = true;
    uint32_t maxAnimatedEntities = 1000;
    uint32_t maxActiveLayers = 8;
    float defaultBlendTime = 0.25f;
    float ikSolverIterations = 10;
    std::string animationCachePath = "Cache/Animations";
};

class RuntimeAnimationSystem {
public:
    static RuntimeAnimationSystem& Get();
    
    void Initialize(const RuntimeAnimationConfig& config = {});
    void Shutdown();
    
    void Update();
    
    AnimationHandle PlayAnimation(EntityHandle entity, const std::string& animName, const PlaybackParams& params = {});
    void StopAnimation(AnimationHandle handle);
    void PauseAnimation(AnimationHandle handle);
    void ResumeAnimation(AnimationHandle handle);
    
    void SetBlendWeight(AnimationHandle handle, float weight);
    void SetPlaybackSpeed(AnimationHandle handle, float speed);
    void SetTimePosition(AnimationHandle handle, float time);
    
    void AddAnimationLayer(EntityHandle entity, const std::string& layerName);
    void RemoveAnimationLayer(EntityHandle entity, const std::string& layerName);
    void SetLayerWeight(EntityHandle entity, const std::string& layerName, float weight);
    
    void EnableIK(EntityHandle entity, const std::string& chainName);
    void DisableIK(EntityHandle entity, const std::string& chainName);
    void SetIKTarget(EntityHandle entity, const std::string& chainName, const Vector3& target);
    
    void RegisterAnimationEvent(const std::string& eventName, const AnimationEventCallback& callback);
    void UnregisterAnimationEvent(const std::string& eventName);
    
    AnimationState GetAnimationState(AnimationHandle handle) const;
    float GetAnimationProgress(AnimationHandle handle) const;
    const RuntimeAnimationStats& GetStats() const { return m_Stats; }

private:
    RuntimeAnimationSystem() = default;
    
    RuntimeAnimationConfig m_Config;
    RuntimeAnimationStats m_Stats;
    std::unordered_map<AnimationHandle, AnimationInstance> m_Animations;
    std::unordered_map<EntityHandle, AnimatedEntityData> m_AnimatedEntities;
    std::unordered_map<std::string, AnimationEventCallback> m_EventCallbacks;
    std::unique_ptr<IAnimationProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 