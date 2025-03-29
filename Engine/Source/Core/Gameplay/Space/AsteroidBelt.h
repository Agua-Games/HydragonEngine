/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AsteroidBelt.h
 * @brief Header file for the AsteroidBelt class.
 * 
 * ARCHITECTURAL NOTES:
 * - AsteroidBelt is a class that represents an asteroid belt in Hydragon.
 * - It is used to represent any asteroid belt in the game world, such as the asteroid belt in the solar system, etc.
 * - It supports interactive features, such as mining, exploration, and trading. Also supports two-way messaging with other asteroid belts, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct AsteroidBeltInfo : public NodeInfo {
    AsteroidBeltInfo() {
        nodeType = "Gameplay/AsteroidBelt";
        
        inputs = {
            "asteroidBeltType",  // Type of asteroid belt (solar system, etc.)
            "asteroidBeltData",  // Asteroid belt data
            "environment",       // Environment data
            "characterData",     // Character data
            "asteroidBeltState"  // Asteroid belt state
        };
        
        outputs = {
            "asteroidBeltStatus",  // Asteroid belt status
            "asteroidBeltMetrics"  // Asteroid belt performance metrics
        };
    }
};

class AsteroidBelt : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AsteroidBelt(const AsteroidBeltInfo& info = AsteroidBeltInfo())
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
    ~AsteroidBelt() = default;     // Default destructor
};

} // namespace hd
