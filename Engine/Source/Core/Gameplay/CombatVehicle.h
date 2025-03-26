/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatVehicle.h
 * @brief Header file for the CombatVehicle class.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatVehicle is a class that represents a combat vehicle in Hydragon.
 * - It is used to represent any combat vehicle in the game world, such as tanks, helicopters, jets, etc.
 * - It supports interactive features, such as driving, steering, and firing. Also supports two-way messaging with other combat vehicles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CombatVehicleInfo : public NodeInfo {
    CombatVehicleInfo() {
        nodeType = "Gameplay/CombatVehicle";
        
        inputs = {
            "model",        // Model of the combat vehicle
            "position",     // Position of the combat vehicle
            "rotation",     // Rotation of the combat vehicle
            "scale",        // Scale of the combat vehicle
            "material",     // Material of the combat vehicle
            "animation",    // Animation of the combat vehicle
            "physics",      // Physics of the combat vehicle
            "collision",    // Collision of the combat vehicle
            "script"        // Script of the combat vehicle
        };
        
        outputs = {
            "model",        // Model of the combat vehicle
            "position",     // Position of the combat vehicle
            "rotation",     // Rotation of the combat vehicle
            "scale",        // Scale of the combat vehicle
            "material",     // Material of the combat vehicle
            "animation",    // Animation of the combat vehicle
            "physics",      // Physics of the combat vehicle
            "collision",    // Collision of the combat vehicle
            "script"        // Script of the combat vehicle
        };
    }
};

class CombatVehicle : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CombatVehicle(const CombatVehicleInfo& info = CombatVehicleInfo())
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
    ~CombatVehicle() = default;     // Default destructor
};

} // namespace hd
