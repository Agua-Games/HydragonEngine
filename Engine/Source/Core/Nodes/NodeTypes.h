/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Node system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <any>

namespace Hydragon::Nodes {

using GraphHandle = uint32_t;
using NodeHandle = uint32_t;

enum class NodeCategory {
    Math,
    Logic,
    Flow,
    Data,
    Asset,
    AI,
    Custom
};

struct GraphDesc {
    std::string name;
    bool isPersistent = false;
    bool autoExecute = false;
    float updateRate = 60.0f;
};

struct NodeDesc {
    NodeCategory category;
    std::string type;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::any defaultValue;
};

struct NodeStats {
    uint32_t activeGraphs;
    uint32_t totalNodes;
    uint32_t totalConnections;
    float averageExecutionTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Nodes 