/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Graph algorithm type definitions
 */

#pragma once
#include "../GraphTypes.h"
#include <vector>

namespace Hydragon::Graph {

struct PathResult {
    std::vector<NodeHandle> path;
    float totalCost;
    uint32_t visitedNodes;
    bool success;
};

struct LayoutParams {
    float springConstant = 1.0f;
    float repulsionConstant = 1.0f;
    uint32_t iterations = 100;
    float minDistance = 1.0f;
    bool useGravity = true;
};

struct ComponentInfo {
    uint32_t componentId;
    uint32_t nodeCount;
    uint32_t edgeCount;
    float density;
};

struct AlgorithmStats {
    uint32_t pathfindingCalls;
    uint32_t analysisCalls;
    uint32_t optimizationCalls;
    float averagePathLength;
    float averageProcessingTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Graph 