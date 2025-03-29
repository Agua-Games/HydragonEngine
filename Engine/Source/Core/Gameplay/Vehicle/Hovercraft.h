/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Hovercraft.h
 * @brief Header file for the Hovercraft class.
 * 
 * ARCHITECTURAL NOTES:
 * - Hovercraft is a class that represents a hovercraft in Hydragon.
 * - It is used to represent any hovercraft in the game world, such as military hovercraft, civilian hovercraft, etc.
 * - It supports interactive features, such as driving, steering, and hovering. Also supports two-way messaging with other hovercrafts, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Combat/CombatVehicle.h"

namespace hd {

struct HovercraftInfo : public VehicleInfo {
    HovercraftInfo() {
        nodeType = "Gameplay/Hovercraft";
        
        inputs = {
            "model",        // Model of the hovercraft
            "position",     // Position of the hovercraft
            "rotation",     // Rotation of the hovercraft
            "scale",        // Scale of the hovercraft
            "material",     // Material of the hovercraft
            "animation",    // Animation of the hovercraft
            "physics",      // Physics of the hovercraft
            "collision",    // Collision of the hovercraft
            "script"        // Script of the hovercraft
        };
        
        outputs = {
            "status",       // Status of the hovercraft
            "metrics"       // Performance metrics of the hovercraft
        };
    }
};

class Hovercraft : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Hovercraft(const HovercraftInfo& info = HovercraftInfo())
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
    ~Hovercraft() = default;     // Default destructor
};

} // namespace hd
