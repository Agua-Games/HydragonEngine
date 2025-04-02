/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file OpenWorldMechanics.h
 * @brief Header file for the OpenWorldMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - OpenWorldMechanics is a class that represents open world mechanics in Hydragon.
 * - It is used to represent any open world mechanics in the game world, such as exploration, discovery, etc.
 * - It supports interactive features, such as exploring, discovering, and interacting with the open world. Also supports two-way messaging with other open world, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "Scene.h"

namespace hd {
struct OpenWorldMechanicsInfo : public NodeInfo {
    OpenWorldMechanicsInfo() {
        nodeType = "Gameplay/OpenWorldMechanics";
        
        inputs = {
            "openWorldMechanicsData",// Open world mechanics data
            "environment",           // Environment data
            "characterData",         // Character data
            "openWorldMechanicsState"// Open world mechanics state
        };
        
        outputs = {
            "openWorldMechanicsStatus",  // Open world mechanics status
            "openWorldMechanicsMetrics"  // Open world mechanics performance metrics
        };
    }
};

class OpenWorldMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit OpenWorldMechanics(const OpenWorldMechanicsInfo& info = OpenWorldMechanicsInfo())
        : Node(info) {} // Default constructor for OpenWorldMechanics class
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~OpenWorldMechanics() = default;     // Default destructor
};

} // namespace hd
