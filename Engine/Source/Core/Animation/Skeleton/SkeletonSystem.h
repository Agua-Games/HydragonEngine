/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Skeleton management and skinning system
 */

#pragma once
#include "SkeletonTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Animation {

class SkeletonSystem {
public:
    struct SkeletonConfig {
        uint32_t maxSkeletons = 500;
        uint32_t maxBonesPerSkeleton = 256;
        bool enableGPUSkinning = true;
        bool enableBoneVisualization = true;
        size_t gpuBufferSize = 64 * 1024 * 1024;  // 64MB
    };

    static SkeletonSystem& Get();
    
    void Initialize(const SkeletonConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void UpdateSkeleton(SkeletonHandle handle);
    void UpdateSkinning(SkeletonHandle handle);
    
    void SetBoneTransform(SkeletonHandle skeleton, BoneHandle bone, const Transform& transform);
    Transform GetBoneTransform(SkeletonHandle skeleton, BoneHandle bone) const;
    
    void EnableGPUSkinning(SkeletonHandle handle, bool enable);
    void UpdateGPUBuffers(SkeletonHandle handle);
    
    const SkeletonStats& GetStats() const { return m_Stats; }

private:
    SkeletonSystem() = default;
    
    SkeletonConfig m_Config;
    SkeletonStats m_Stats;
    std::unique_ptr<ISkinningProcessor> m_Processor;
    std::vector<Skeleton> m_Skeletons;
    bool m_Initialized = false;
};

} // namespace Hydragon::Animation 