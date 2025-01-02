/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene graph type definitions
 */

#pragma once
#include "Core/Math/MathTypes.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Scene {

using NodeHandle = uint64_t;

enum class NodeFlags {
    None = 0,
    Static = 1 << 0,
    Dynamic = 1 << 1,
    Visible = 1 << 2,
    Culled = 1 << 3,
    Occluded = 1 << 4,
    Selected = 1 << 5,
    Highlighted = 1 << 6
};

struct NodeDesc {
    std::string name;
    Transform localTransform;
    BoundingVolume bounds;
    uint32_t layer = 0;
    NodeFlags flags = NodeFlags::Visible;
};

struct Node {
    NodeDesc desc;
    NodeHandle parent;
    std::vector<NodeHandle> children;
    Transform worldTransform;
    BoundingVolume worldBounds;
    bool dirty = false;
};

struct GraphStats {
    uint32_t totalNodes;
    uint32_t visibleNodes;
    uint32_t culledNodes;
    uint32_t dynamicNodes;
    uint32_t staticNodes;
    float updateTimeMs;
    float cullingTimeMs;
};

} // namespace Hydragon::Scene 