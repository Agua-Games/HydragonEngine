/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Settlement.h
 * @brief Header file for the Settlement class.
 * 
 * ARCHITECTURAL NOTES:
 * - Settlement is a class that represents a settlement in Hydragon.
 * - It is used to represent any settlement in the game world, such as cities, towns, villages, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other settlements, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SettlementInfo : public BuildingInfo {
    SettlementInfo() {
        nodeType = "Gameplay/Settlement";
        
        inputs = {
            "model",        // Model of the settlement
            "position",     // Position of the settlement
            "rotation",     // Rotation of the settlement
            "scale",        // Scale of the settlement
            "material",     // Material of the settlement
            "animation",    // Animation of the settlement
            "physics",      // Physics of the settlement
            "collision",    // Collision of the settlement
            "script"        // Script of the settlement
        };
        
        outputs = {
            "model",        // Model of the settlement
            "position",     // Position of the settlement
            "rotation",     // Rotation of the settlement
            "scale",        // Scale of the settlement
            "material",     // Material of the settlement
            "animation",    // Animation of the settlement
            "physics",      // Physics of the settlement
            "collision",    // Collision of the settlement
            "script"        // Script of the settlement
        };
    }
};

class Settlement : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Settlement(const SettlementInfo& info = SettlementInfo())
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
    ~Settlement() = default;     // Default destructor
};

} // namespace hd
