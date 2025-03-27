/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Equipment.h
 * @brief Header file for the Equipment class.
 * 
 * ARCHITECTURAL NOTES:
 * - Equipment is a class that represents equipment in Hydragon.
 * - It is used to represent any equipment in the game world, such as armor, weapons, tools, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other equipment, environment, character, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct EquipmentInfo : public NodeInfo {
    EquipmentInfo() {
        nodeType = "Gameplay/Equipment";
        
        inputs = {
            "equipmentType",  // Type of equipment (armor, weapon, tool, etc.) (armor, weapon, tool, etc.)
            "equipmentData",  // Equipment data (stats, etc.)
            "environment",    // Environment data (stats, etc.)
            "characterData",
            "equipmentState"
        };
        
        outputs = {
            "equipmentStatus",
            "equipmentMetrics"  // Equipment performance metrics
        };
    }
};

class Equipment : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Equipment(const EquipmentInfo& info = EquipmentInfo())
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
    ~Equipment() = default;     // Default destructor
};

} // namespace hd
