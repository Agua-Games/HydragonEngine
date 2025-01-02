/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Skeleton system type definitions
 */

#pragma once
#include "Core/Math/Transform.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Animation {

struct BoneInfluence {
    BoneHandle bone;
    float weight;
};

struct VertexSkinData {
    std::vector<BoneInfluence> influences;
    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
};

struct SkinningStats {
    uint32_t verticesProcessed;
    uint32_t bonesUpdated;
    uint32_t gpuBufferUpdates;
    float skinningTimeMs;
    size_t gpuMemoryUsed;
};

struct SkeletonStats {
    uint32_t activeSkeletons;
    uint32_t totalBones;
    uint32_t gpuSkinningInstances;
    float updateTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Animation 