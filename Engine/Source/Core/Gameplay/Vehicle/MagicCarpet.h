/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MagicCarpet.h
 * @brief Header file for the MagicCarpet class.
 * 
 * ARCHITECTURAL NOTES:
 * - MagicCarpet is a class that represents a magic carpet in Hydragon.
 * - It is used to represent any magic carpet in the game world, such as flying carpets, levitating carpets, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other magic carpets, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct MagicCarpetInfo : public VehicleInfo {
    MagicCarpetInfo() {
        nodeType = "Gameplay/MagicCarpet";
        
        inputs = {
            "model",        // Model of the magic carpet
            "position",     // Position of the magic carpet
            "rotation",     // Rotation of the magic carpet
            "scale",        // Scale of the magic carpet
            "material",     // Material of the magic carpet
            "animation",    // Animation of the magic carpet
            "physics",      // Physics of the magic carpet
            "collision",    // Collision of the magic carpet
            "script"        // Script of the magic carpet
        };
        
        outputs = {
            "magicCarpetStatus",   // Magic carpet status
            "magicCarpetMetrics"   // Magic carpet performance metrics
        };
    }
};

class MagicCarpet : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit MagicCarpet(const MagicCarpetInfo& info = MagicCarpetInfo())
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
    ~MagicCarpet() = default;     // Default destructor
};

} // namespace hd

