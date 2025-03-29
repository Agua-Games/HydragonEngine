/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Aircraft.h
 * @brief Header file for the Aircraft class.
 * 
 * ARCHITECTURAL NOTES:
 * - Aircraft is a class that represents an aircraft in Hydragon.
 * - It is used to represent any aircraft in the game world, such as planes, helicopters, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other aircraft, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct AircraftInfo : public CombatVehicleInfo {
    AircraftInfo() {
        nodeType = "Gameplay/Aircraft";
        
        inputs = {
            "model",        // Model of the aircraft
            "position",     // Position of the aircraft
            "rotation",     // Rotation of the aircraft
            "scale",        // Scale of the aircraft
            "material",     // Material of the aircraft
            "animation",    // Animation of the aircraft
            "physics",      // Physics of the aircraft
            "collision",    // Collision of the aircraft
            "script"        // Script of the aircraft
        };
        
        outputs = {
            "status",       // Status of the aircraft
            "metrics"       // Performance metrics of the aircraft
        };
    }
};

class Aircraft : public CombatVehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Aircraft(const AircraftInfo& info = AircraftInfo())
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
    ~Aircraft() = default;     // Default destructor
};

} // namespace hd   
