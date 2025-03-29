/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ReentryVehicle.h
 * @brief Header file for the ReentryVehicle class.
 * 
 * ARCHITECTURAL NOTES:
 * - ReentryVehicle is a class that represents a reentry vehicle in Hydragon.
 * - It is used to represent any reentry vehicle in the game world, such as space shuttles, space capsules, etc.
 * - It supports interactive features, such as launching, steering, and landing. Also supports two-way messaging with other reentry vehicles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct ReentryVehicleInfo : public VehicleInfo {
    ReentryVehicleInfo() {
        nodeType = "Gameplay/ReentryVehicle";
        
        inputs = {
            "reentryVehicleType",  // Type of reentry vehicle (space shuttle, space capsule, etc.)
            "reentryVehicleData",  // Reentry vehicle data
            "environment",         // Environment data
            "characterData",       // Character data
            "reentryVehicleState"  // Reentry vehicle state
        };
        
        outputs = {
            "reentryVehicleStatus",  // Reentry vehicle status
            "reentryVehicleMetrics"  // Reentry vehicle performance metrics
        };
    }
};

class ReentryVehicle : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit ReentryVehicle(const ReentryVehicleInfo& info = ReentryVehicleInfo())
        : Vehicle(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void launch();
    void steer();
    void land();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ReentryVehicle() = default;     // Default destructor
};

} // namespace hd
