/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceWagon.h
 * @brief Header file for the SpaceWagon class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceWagon is a class that represents a space wagon in Hydragon.
 * - It is used to represent any space wagon in the game world, such as space wagons, space carts, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other space wagons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct SpaceWagonInfo : public VehicleInfo {
    SpaceWagonInfo() {
        nodeType = "Gameplay/SpaceWagon";
        
        inputs = {
            "spaceWagonType",  // Type of space wagon (space wagon, space cart, etc.)
            "spaceWagonData",  // Space wagon data
            "environment",     // Environment data
            "characterData",   // Character data
            "spaceWagonState"  // Space wagon state
        };
        
        outputs = {
            "spaceWagonStatus",  // Space wagon status
            "spaceWagonMetrics"  // Space wagon performance metrics
        };
    }
};

class SpaceWagon : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpaceWagon(const SpaceWagonInfo& info = SpaceWagonInfo())
        : Vehicle(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void drive();
    void steer();
    void brake();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceWagon() = default;     // Default destructor  
};

} // namespace hd
