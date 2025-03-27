/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodePool.h
 * @brief NodePool is a class that manages a pool of nodes. It is a type of Object.
 * 
 * ARCHITECTURAL NOTES:
 * - It provides a common interface for all node pools in the engine, like textures, models, shaders, audio, fonts, etc.
 * - NodePool is a type of Object, so it can be serialized and deserialized.
 * - NodePool is a type of Node, so it can be connected to other nodes in the engine's node graph.
 */
#pragma once
#include "Engine.h"
#include "Node.h"

namespace hd {
struct NodePoolInfo : public NodeInfo {
    NodePoolInfo() {
        nodeType = "Node/NodePool";
        inputs = {
            "nodePoolType",   // Type of node pool (texture, model, shader, audio, font, etc.)
            "nodePoolPath",   // Path to the node pool
            "nodePoolData",   // Node pool data
            "nodePoolMetadata"// Node pool metadata
        };
        outputs = {
            "loadedNodePool", // Loaded node pool
            "nodePoolStatus", // Node pool loading status
            "nodePoolMetadata"// Node pool metadata
        };
    }
};

/**
 * @class NodePool
 * @brief Represents a node pool in the engine's node graph.
 */
class NodePool : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit NodePool(const NodePoolInfo& info = NodePoolInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~NodePool() = default;     // Default destructor
};

} // namespace hd
