/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Fortress.h
 * @brief Header file for the Fortress class.
 * 
 * ARCHITECTURAL NOTES:
 * - Fortress is a class that represents a fortress in Hydragon.
 * - It is used to represent any fortress in the game world, such as castles, forts, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other fortresses, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct FortressInfo : public BuildingInfo {
    FortressInfo() {
        nodeType = "Gameplay/Fortress";
        
        inputs = {
            "model",        // Model of the fortress
            "position",     // Position of the fortress
            "rotation",     // Rotation of the fortress
            "scale",        // Scale of the fortress
            "material",     // Material of the fortress
            "animation",    // Animation of the fortress
            "physics",      // Physics of the fortress
            "collision",    // Collision of the fortress
            "script"        // Script of the fortress
        };
        
        outputs = {
            "model",        // Model of the fortress
            "position",     // Position of the fortress
            "rotation",     // Rotation of the fortress
            "scale",        // Scale of the fortress
            "material",     // Material of the fortress
            "animation",    // Animation of the fortress
            "physics",      // Physics of the fortress
            "collision",    // Collision of the fortress
            "script"        // Script of the fortress
        };
    }
};

class Fortress : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Fortress(const FortressInfo& info = FortressInfo())
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
    ~Fortress() = default;     // Default destructor
};

} // namespace hd
