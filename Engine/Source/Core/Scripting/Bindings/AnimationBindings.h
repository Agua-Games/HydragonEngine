/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Animation system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Animation/AnimationSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct AnimationBindingsConfig {
    bool enableBlending = true;
    bool enableIK = true;
    bool enableMorphTargets = true;
    bool enableRagdoll = true;
    uint32_t maxAnimators = 1000;
    uint32_t maxClips = 500;
    uint32_t maxLayers = 8;
    std::string animationDataPath = "Data/Animation";
};

class AnimationBindings {
public:
    static AnimationBindings& Get();
    
    void Initialize(const AnimationBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Animator management
    AnimatorHandle CreateAnimator(const AnimatorDesc& desc);
    void DestroyAnimator(AnimatorHandle handle);
    void SetSkeleton(AnimatorHandle handle, const SkeletonDesc& desc);
    
    // Animation clips
    ClipHandle CreateClip(const std::string& name, const ClipDesc& desc);
    void DestroyClip(ClipHandle handle);
    void SetClipSpeed(ClipHandle handle, float speed);
    void SetClipWeight(ClipHandle handle, float weight);
    
    // Playback control
    void Play(AnimatorHandle handle, ClipHandle clip, const PlaybackParams& params = {});
    void Stop(AnimatorHandle handle, ClipHandle clip);
    void Pause(AnimatorHandle handle, ClipHandle clip);
    void Resume(AnimatorHandle handle, ClipHandle clip);
    void CrossFade(AnimatorHandle handle, ClipHandle fromClip, ClipHandle toClip, float duration);
    
    // Layer management
    void AddLayer(AnimatorHandle handle, const LayerDesc& desc);
    void RemoveLayer(AnimatorHandle handle, uint32_t layerIndex);
    void SetLayerWeight(AnimatorHandle handle, uint32_t layerIndex, float weight);
    void SetLayerMask(AnimatorHandle handle, uint32_t layerIndex, const BoneMask& mask);
    
    // IK control
    void SetIKTarget(AnimatorHandle handle, const std::string& chainName, const Vector3& target);
    void SetIKRotation(AnimatorHandle handle, const std::string& chainName, const Quaternion& rotation);
    void SetIKEnabled(AnimatorHandle handle, const std::string& chainName, bool enabled);
    
    // Morph targets
    void SetMorphWeight(AnimatorHandle handle, const std::string& targetName, float weight);
    void BlendMorphTargets(AnimatorHandle handle, const std::vector<MorphBlend>& blends);
    
    // Ragdoll physics
    void EnableRagdoll(AnimatorHandle handle, bool enable);
    void SetRagdollBlend(AnimatorHandle handle, float blend);
    void ApplyRagdollForce(AnimatorHandle handle, const std::string& boneName, const Vector3& force);
    
    // State queries
    bool IsPlaying(AnimatorHandle handle, ClipHandle clip) const;
    float GetPlaybackTime(AnimatorHandle handle, ClipHandle clip) const;
    float GetClipDuration(ClipHandle handle) const;
    AnimationState GetAnimationState(AnimatorHandle handle) const;
    
    // Debug utilities
    void EnableDebugVisualization(bool enable);
    void SetDebugBone(AnimatorHandle handle, const std::string& boneName);
    void DumpAnimatorState(AnimatorHandle handle, const std::string& path);
    
    const AnimationBindingsStats& GetStats() const { return m_Stats; }

private:
    AnimationBindings() = default;
    
    AnimationBindingsConfig m_Config;
    AnimationBindingsStats m_Stats;
    std::unordered_map<AnimatorHandle, AnimatorInstance> m_Animators;
    std::unordered_map<ClipHandle, ClipInstance> m_Clips;
    std::unordered_map<std::string, IKChainInstance> m_IKChains;
    std::unordered_map<std::string, MorphTargetInstance> m_MorphTargets;
    std::unique_ptr<Animation::IAnimationProcessor> m_Processor;
    std::unique_ptr<Animation::IIKSolver> m_IKSolver;
    std::unique_ptr<Animation::IMorphController> m_MorphController;
    std::unique_ptr<Animation::IRagdollController> m_RagdollController;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 