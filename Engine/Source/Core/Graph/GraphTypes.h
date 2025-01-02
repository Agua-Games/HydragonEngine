/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Graph system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Graph {

using GraphHandle = uint32_t;
using NodeHandle = uint32_t;
using EdgeHandle = uint32_t;

enum class GraphType {
    Directed,
    Undirected,
    Tree,
    DAG,
    FlowNetwork
};

enum class TraversalType {
    BreadthFirst,
    DepthFirst,
    Dijkstra,
    AStar,
    TopologicalSort
};

struct GraphDesc {
    std::string name;
    GraphType type;
    bool persistent = false;
    bool allowCycles = true;
    uint32_t expectedSize = 1000;
};

struct NodeDesc {
    std::string name;
    uint32_t weight = 1;
    bool active = true;
    std::vector<uint8_t> userData;
};

struct EdgeDesc {
    float weight = 1.0f;
    bool bidirectional = false;
    bool active = true;
    std::vector<uint8_t> userData;
};

using NodeVisitor = std::function<void(NodeHandle, const NodeDesc&)>;
using EdgeVisitor = std::function<void(EdgeHandle, const EdgeDesc&)>;

struct TraversalParams {
    TraversalType type;
    NodeHandle startNode;
    NodeVisitor nodeVisitor;
    EdgeVisitor edgeVisitor;
    bool parallel = false;
};

struct OptimizationParams {
    bool minimizeEdges = true;
    bool balanceNodes = true;
    bool removeIsolatedNodes = true;
    float weightThreshold = 0.1f;
};

struct GraphStats {
    uint32_t totalGraphs;
    uint32_t totalNodes;
    uint32_t totalEdges;
    uint32_t traversalCount;
    float averageTraversalTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Graph 