/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Parachute.h
 * @brief Header file for the Parachute class.
 * 
 * ARCHITECTURAL NOTES:
 * - Parachute is a class that represents a parachute in Hydragon.
 * - It is used to represent any parachute in the game world, such as skydiving parachute, military parachute, etc.
 * - It supports interactive features, such as deploying, steering, and landing. Also supports two-way messaging with other parachute, environment, character, etc.
 */
#pragma once
#include "Core.h"
#include "Node.h"
#include "Vehicle.h"

namespace hd {

struct ParachuteInfo : public VehicleInfo {
    ParachuteInfo() {
        nodeType = "Gameplay/Parachute";
        
        inputs = {
            "parachuteType",  // Type of parachute (skydiving, military, etc.)
            "parachuteData",  // Parachute data
            "environment",    // Environment data
            "characterData",  // Character data
            "Parachutetate"  // Parachute state
        };
        
        outputs = {
            "Parachutetatus", // Parachute status
            "parachuteMetrics" // Parachute performance metrics
        };
    }
};

class Parachute : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Parachute(const ParachuteInfo& info = ParachuteInfo())
        : Vehicle(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Parachute() = default;     // Default destructor
};

} // namespace hd
