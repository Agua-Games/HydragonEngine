/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ForLoop.h
 * @brief Header file for the ForLoop class.
 * 
 * ARCHITECTURAL NOTES:
 * - ForLoop is a class that represents a for loop in Hydragon.
 * - It is used to represent and process for loops.
 * - It uses the Vulkan API for for loop processing.
 * 
 * @todo Declare-define all default member variables.
 */
#pragma once
#include "Engine.h"
#include "Node.h"

namespace hd {

struct ForLoopInfo : public NodeInfo {
    ForLoopInfo() {
        NodeType = "Logic/ForLoop";
        
        inputs = {
            "Start",  // Start value
            "End",    // End value
            "Step"    // Step value
        };
        
        outputs = {
            "Result" // Result of the for loop
        };
    }
};

class ForLoop : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ForLoop(const ForLoopInfo& info = ForLoopInfo())
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
    ~ForLoop() = default;     // Default destructor
};

} // namespace hd
