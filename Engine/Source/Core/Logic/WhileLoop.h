/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WhileLoop.h
 * @brief Header file for the WhileLoop class.
 * 
 * ARCHITECTURAL NOTES:
 * - WhileLoop is a class that represents a while loop in Hydragon.
 * - It is used to represent and process while loops.
 * - It uses the Vulkan API for while loop processing.
 */
#pragma once
#include "Engine.h"
#include "Node.h"

namespace hd {

struct WhileLoopInfo : public NodeInfo {
    WhileLoopInfo() {
        NodeType = "Logic/WhileLoop";
        
        inputs = {
            "Condition",  // Condition to evaluate
            "Body"        // Body of the while loop
        };
        
        outputs = {
            "Result" // Result of the while loop
        };
    }
};

class WhileLoop : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit WhileLoop(const WhileLoopInfo& info = WhileLoopInfo())
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
    ~WhileLoop() = default;     // Default destructor
};

} // namespace hd
