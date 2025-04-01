/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Scooter.h
 * @brief Header file for the Scooter class.
 * 
 * ARCHITECTURAL NOTES:
 * - Scooter is a class that represents a scooter in Hydragon.
 * - It is used to represent any scooter in the game world, such as electric scooters, gas scooters, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other scooters, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct ScooterInfo : public VehicleInfo {
    ScooterInfo() {
        nodeType = "Gameplay/Scooter";
        
        inputs = {
            "model",        // Model of the scooter
            "position",     // Position of the scooter
            "rotation",     // Rotation of the scooter
            "scale",        // Scale of the scooter
            "material",     // Material of the scooter
            "animation",    // Animation of the scooter
            "physics",      // Physics of the scooter
            "collision",    // Collision of the scooter
            "script"        // Script of the scooter
        };
        
        outputs = {
            "scooterStatus", // Scooter status
            "scooterMetrics" // Scooter performance metrics
        };
    }
};

class Scooter : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Scooter(const ScooterInfo& info = ScooterInfo())
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
    ~Scooter() = default;     // Default destructor
};

} // namespace hd

