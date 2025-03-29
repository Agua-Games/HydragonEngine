/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Helicopter.h
 * @brief Header file for the Helicopter class.
 * 
 * ARCHITECTURAL NOTES:
 * - Helicopter is a class that represents a helicopter in Hydragon.
 * - It is used to represent any helicopter in the game world, such as attack helicopters, transport helicopters, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other helicopters, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct HelicopterInfo : public CombatVehicleInfo {
    HelicopterInfo() {
        nodeType = "Gameplay/Helicopter";
        
        inputs = {
            "model",        // Model of the helicopter
            "position",     // Position of the helicopter
            "rotation",     // Rotation of the helicopter
            "scale",        // Scale of the helicopter
            "material",     // Material of the helicopter
            "animation",    // Animation of the helicopter
            "physics",      // Physics of the helicopter
            "collision",    // Collision of the helicopter
            "script"        // Script of the helicopter
        };
        
        outputs = {
            "status",       // Status of the helicopter
            "metrics"       // Performance metrics of the helicopter
        };
    }
};

class Helicopter : public CombatVehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Helicopter(const HelicopterInfo& info = HelicopterInfo())
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
    ~Helicopter() = default;     // Default destructor
};

} // namespace hd
