/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ParentLink.h
 * @brief Header file for the ParentLink class.
 * 
 * ARCHITECTURAL NOTES:
 * - ParentLink is a class that represents a parent link in Hydragon.
 * - It is used to link a node to another node as a parent transform.
 * - It supports parenting only translation, rotation, or scale, or any combination of them. And also supports custom transformations.
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

struct ParentLinkInfo : public NodeInfo {
    ParentLinkInfo() {
        NodeType = "Logic/ParentLink";
        
        inputs = {
            "SourceNode",  // Node to parent
            "TargetNode",  // Node to parent to
            "Offset"       // Optional offset
        };
        
        outputs = {
            "ParentedNode" // Parented node
        };
    }
};

class ParentLink : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ParentLink(const ParentLinkInfo& info = ParentLinkInfo())
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
    ~ParentLink() = default;     // Default destructor
};

} // namespace hd
