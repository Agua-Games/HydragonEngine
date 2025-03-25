/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FollowLink.h
 * @brief Header file for the FollowLink class.
 * 
 * ARCHITECTURAL NOTES:
 * - FollowLink is a class that represents a follow link in Hydragon.
 * - It is used to follow a node to another node.
 * - It supports linking arbitrary data types, such as position, rotation, scale, color, etc. And has built-in support for modulation with
 * procedural features, using ProceduralPatternData.
 * - It supports linking multiple nodes to a single node.
 * - It has native support for WavePhysics and has physics properties and features, like elasticEnergy/spring, damping, tension, bending, etc.
 * - It supports different interpolation modes, such as linear, cubic, and spline interpolation.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It offers built-in support for procedural modification of imported nodes, and for procedural node generation.
 */
#pragma once
#include "Node.h"
#include "Animation.h"

namespace hd {

struct FollowLinkInfo : public NodeInfo {
    FollowLinkInfo() {
        NodeType = "Logic/FollowLink";
        
        inputs = {
            "SourceNode",  // Node to follow
            "TargetNode",  // Node to follow to
            "Offset"       // Optional offset
        };
        
        outputs = {
            "FollowedNode" // Followed node
        };
    }
};

class FollowLink : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit FollowLink(const FollowLinkInfo& info = FollowLinkInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FollowLink() = default;     // Default destructor
};

} // namespace hd
