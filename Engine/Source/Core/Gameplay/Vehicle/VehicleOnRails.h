/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VehicleOnRails.h
 * @brief Header file for the VehicleOnRails class.
 * 
 * ARCHITECTURAL NOTES:
 * - VehicleOnRails is a class that represents a vehicle on rails in Hydragon.
 * - It is used to represent any vehicle on rails in the game world, such as trains, trams, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other vehicles on rails, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct VehicleOnRailsInfo : public VehicleInfo {
    VehicleOnRailsInfo() {
        nodeType = "Gameplay/VehicleOnRails";
        
        inputs = {
            "vehicleOnRailsType",  // Type of vehicle on rails (train, tram, etc.)
            "vehicleOnRailsData",  // Vehicle on rails data
            "environment",         // Environment data
            "characterData",       // Character data
            "vehicleOnRailsState"  // Vehicle on rails state
        };
        
        outputs = {
            "vehicleOnRailsStatus",  // Vehicle on rails status
            "vehicleOnRailsMetrics"  // Vehicle on rails performance metrics
        };
    }
};

class VehicleOnRails : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit VehicleOnRails(const VehicleOnRailsInfo& info = VehicleOnRailsInfo())
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
    ~VehicleOnRails() = default;     // Default destructor
};

} // namespace hd
