/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BallisticVehicle.h
 * @brief Header file for the BallisticVehicle class.
 * 
 * ARCHITECTURAL NOTES:
 * - BallisticVehicle is a class that represents a ballistic vehicle in Hydragon.
 * - It is used to represent any ballistic vehicle in the game world, such as rockets, missiles, etc.
 * - It supports interactive features, such as launching, steering, and exploding. Also supports two-way messaging with other ballistic vehicles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct BallisticVehicleInfo : public VehicleInfo {
    BallisticVehicleInfo() {
        nodeType = "Gameplay/BallisticVehicle";
        
        inputs = {
            "model",        // Model of the ballistic vehicle
            "position",     // Position of the ballistic vehicle
            "rotation",     // Rotation of the ballistic vehicle
            "scale",        // Scale of the ballistic vehicle
            "material",     // Material of the ballistic vehicle
            "animation",    // Animation of the ballistic vehicle
            "physics",      // Physics of the ballistic vehicle
            "collision",    // Collision of the ballistic vehicle
            "script"        // Script of the ballistic vehicle
        };
        
        outputs = {
            "model",        // Model of the ballistic vehicle
            "position",     // Position of the ballistic vehicle
            "rotation",     // Rotation of the ballistic vehicle
            "scale",        // Scale of the ballistic vehicle
            "material",     // Material of the ballistic vehicle
            "animation",    // Animation of the ballistic vehicle
            "physics",      // Physics of the ballistic vehicle
            "collision",    // Collision of the ballistic vehicle
            "script"        // Script of the ballistic vehicle
        };
    }
};

class BallisticVehicle : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit BallisticVehicle(const BallisticVehicleInfo& info = BallisticVehicleInfo())
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
    ~BallisticVehicle() = default;     // Default destructor
};

} // namespace hd
