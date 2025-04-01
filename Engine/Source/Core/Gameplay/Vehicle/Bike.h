/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Bike.h
 * @brief Header file for the Bike class.
 * 
 * ARCHITECTURAL NOTES:
 * - Bike is a class that represents a bike in Hydragon.
 * - It is used to represent any bike in the game world, such as bicycles, tricycles, etc.
 * - It supports interactive features, such as pedaling, steering, and braking. Also supports two-way messaging with other bikes, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct BikeInfo : public VehicleInfo {
    BikeInfo() {
        nodeType = "Gameplay/Bike";
        
        inputs = {
            "model",        // Model of the bike
            "position",     // Position of the bike
            "rotation",     // Rotation of the bike
            "scale",        // Scale of the bike
            "material",     // Material of the bike
            "animation",    // Animation of the bike
            "physics",      // Physics of the bike
            "collision",    // Collision of the bike
            "script"        // Script of the bike
        };
        
        outputs = {
            "bikeStatus",   // Bike status
            "bikeMetrics"   // Bike performance metrics
        };
    }
};

class Bike : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Bike(const BikeInfo& info = BikeInfo())
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
    ~Bike() = default;     // Default destructor
};

} // namespace hd

