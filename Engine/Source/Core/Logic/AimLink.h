/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AimLink.h
 * @brief Header file for the AimLink class.
 * 
 * ARCHITECTURAL NOTES:
 * - AimLink is a class that represents an aim link in Hydragon.
 * - It is used to aim a node to another node.
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

struct AimLinkInfo : public NodeInfo {
    AimLinkInfo() {
        NodeType = "Logic/AimLink";
        
        inputs = {
            "SourceNode",  // Node to aim
            "TargetNode",  // Node to aim to
            "Offset"       // Optional offset
        };
        
        outputs = {
            "AimedNode" // Aimed node
        };
    }
};

class AimLink : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AimLink(const AimLinkInfo& info = AimLinkInfo())
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
    ~AimLink() = default;     // Default destructor
};

} // namespace hd
