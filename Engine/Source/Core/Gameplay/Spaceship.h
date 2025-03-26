/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Spaceship.h
 * @brief Header file for the Spaceship class.
 * 
 * ARCHITECTURAL NOTES:
 * - Spaceship is a class that represents a spaceship in Hydragon.
 * - It is used to represent any spaceship in the game world, such as spaceships, shuttles, etc.
 * - It supports interactive features, such as flying, steering, and docking. Also supports two-way messaging with other spaceships, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SpaceshipInfo : public VehicleInfo {
    SpaceshipInfo() {
        nodeType = "Gameplay/Spaceship";
        
        inputs = {
            "model",        // Model of the spaceship
            "position",     // Position of the spaceship
            "rotation",     // Rotation of the spaceship
            "scale",        // Scale of the spaceship
            "material",     // Material of the spaceship
            "animation",    // Animation of the spaceship
            "physics",      // Physics of the spaceship
            "collision",    // Collision of the spaceship
            "script"        // Script of the spaceship
        };
        
        outputs = {
            "spaceshipStatus", // Spaceship status
            "spaceshipMetrics" // Spaceship performance metrics
        };
    }
};

class Spaceship : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Spaceship(const SpaceshipInfo& info = SpaceshipInfo())
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
    ~Spaceship() = default;     // Default destructor
};

} // namespace hd
