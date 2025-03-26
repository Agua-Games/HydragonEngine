/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file OrientLink.h
 * @brief Header file for the OrientLink class.
 * 
 * ARCHITECTURAL NOTES:
 * - OrientLink is a class that represents an orientation link in Hydragon.
 * - It is used to orient a node to another node.
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

struct OrientLinkInfo : public NodeInfo {
    OrientLinkInfo() {
        NodeType = "Logic/OrientLink";
        
        inputs = {
            "SourceNode",  // Node to orient
            "TargetNode",  // Node to orient to
            "Offset"       // Optional offset
        };
        
        outputs = {
            "OrientedNode" // Oriented node
        };
    }
};

class OrientLink : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit OrientLink(const OrientLinkInfo& info = OrientLinkInfo())
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
    ~OrientLink() = default;     // Default destructor
};

} // namespace hd
