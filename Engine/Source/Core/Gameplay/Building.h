/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Building.h
 * @brief Header file for the Building class.
 * 
 * ARCHITECTURAL NOTES:
 * - Building is a class that represents a building in Hydragon.
 * - It is used to represent any building in the game world, such as houses, offices, factories, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other buildings, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct BuildingInfo : public NodeInfo {
    BuildingInfo() {
        nodeType = "Gameplay/Building";
        
        inputs = {
            "model",        // Model of the building
            "position",     // Position of the building
            "rotation",     // Rotation of the building
            "scale",        // Scale of the building
            "material",     // Material of the building
            "animation",    // Animation of the building
            "physics",      // Physics of the building
            "collision",    // Collision of the building
            "script"        // Script of the building
        };
        
        outputs = {
            "model",        // Model of the building
            "position",     // Position of the building
            "rotation",     // Rotation of the building
            "scale",        // Scale of the building
            "material",     // Material of the building
            "animation",    // Animation of the building
            "physics",      // Physics of the building
            "collision",    // Collision of the building
            "script"        // Script of the building
        };
    }
};

class Building : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Building(const BuildingInfo& info = BuildingInfo())
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
    ~Building() = default;     // Default destructor
};

} // namespace hd

