/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Sailboat.h
 * @brief Header file for the Sailboat class.
 * 
 * ARCHITECTURAL NOTES:
 * - Sailboat is a class that represents a sailboat in Hydragon.
 * - It is used to represent any sailboat in the game world, such as racing sailboats, cruising sailboats, etc.
 * - It supports interactive features, such as sailing, steering, and docking. Also supports two-way messaging with other sailboats, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Boat.h"

namespace hd {

struct SailboatInfo : public BoatInfo {
    SailboatInfo() {
        nodeType = "Gameplay/Sailboat";
        
        inputs = {
            "model",        // Model of the sailboat
            "position",     // Position of the sailboat
            "rotation",     // Rotation of the sailboat
            "scale",        // Scale of the sailboat
            "material",     // Material of the sailboat
            "animation",    // Animation of the sailboat
            "physics",      // Physics of the sailboat
            "collision",    // Collision of the sailboat
            "script"        // Script of the sailboat
        };
        
        outputs = {
            "sailboatStatus", // Sailboat status
            "sailboatMetrics" // Sailboat performance metrics
        };
    }
};

class Sailboat : public Boat {
public:
    // === Allocation, Initialization, Loading ===
    explicit Sailboat(const SailboatInfo& info = SailboatInfo())
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
    ~Sailboat() = default;     // Default destructor
};

} // namespace hd
