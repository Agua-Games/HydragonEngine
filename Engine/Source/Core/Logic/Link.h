/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Link.h
 * @brief Header file for the Link class.
 * 
 * ARCHITECTURAL NOTES:
 * - Link is a class that represents a link in Hydragon.
 * - It is used to link a node to another node.
 * - It is the most basic link type, the base class for all other links. For more advanced specialized links see:
 *      - ParentLink
 *      - PointLink
 *      - OrientLink
 *      - AimLink
 *      - FollowLink
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

struct LinkInfo : public NodeInfo {
    LinkInfo() {
        NodeType = "Logic/Link";
        
        inputs = {
            "SourceNode",  // Node to link
            "TargetNode",  // Node to link to
            "Offset"       // Optional offset
        };
        
        outputs = {
            "LinkedNode" // Linked node
        };
    }
};

class Link : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Link(const LinkInfo& info = LinkInfo())
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
    ~Link() = default;     // Default destructor
};

} // namespace hd
