/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PointLink.h
 * @brief Header file for the PointLink class.
 * 
 * ARCHITECTURAL NOTES:
 * - PointLink is a class that represents a point link in Hydragon.
 * - It is used to link a node to a point.
 * - It supports different interpolation modes, such as linear, cubic, and spline interpolation.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It offers built-in support for procedural modification of imported nodes, and for procedural node generation.
 */
#pragma once
#include "Node.h"
#include "Animation.h"

namespace hd {

struct PointLinkInfo : public NodeInfo {
    PointLinkInfo() {
        NodeType = "Logic/PointLink";
        
        inputs = {
            "SourceNode",  // Node to link
            "TargetPoint", // Point to link to
            "Offset"       // Optional offset
        };
        
        outputs = {
            "LinkedNode" // Linked node
        };
    }
};

class PointLink : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit PointLink(const PointLinkInfo& info = PointLinkInfo())
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
    ~PointLink() = default;     // Default destructor
};

} // namespace hd
