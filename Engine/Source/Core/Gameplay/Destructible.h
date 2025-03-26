/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Destructible.h
 * @brief Header file for the Destructible class.
 * 
 * ARCHITECTURAL NOTES:
 * - Destructible is a class that represents a destructible object in Hydragon.
 * - It is used to represent any destructible object in the game world, such as buildings, vehicles, etc.
 * - It supports interactive features, such as breaking, crumbling, and exploding. Also supports two-way messaging with other destructible objects, environment, character, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct DestructibleInfo : public NodeInfo {
    DestructibleInfo() {
        nodeType = "Gameplay/Destructible";
        
        inputs = {
            "destructibleType",  // Type of destructible object (building, vehicle, etc.)
            "destructibleData",  // Destructible object data
            "environment",       // Environment data
            "characterData",     // Character data
            "destructibleState"  // Destructible object state
        };
        
        outputs = {
            "destructibleStatus",  // Destructible object status
            "destructibleMetrics"  // Destructible object performance metrics
        };
    }
};

class Destructible : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Destructible(const DestructibleInfo& info = DestructibleInfo())
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
    ~Destructible() = default;     // Default destructor
};

} // namespace hd
