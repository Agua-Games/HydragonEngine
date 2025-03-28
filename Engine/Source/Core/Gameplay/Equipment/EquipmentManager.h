/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EquipmentManager.h
 * @brief Header file for the EquipmentManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - EquipmentManager is a singleton class that manages the equipment in the game.
 * - It is responsible for managing the equipment in the game.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other equipment, environment, character, etc.
 */
#pragma once
#include "Equipment.h"

namespace hd {

struct EquipmentManagerInfo : public NodeInfo {
    EquipmentManagerInfo() {
        nodeType = "Gameplay/EquipmentManager";
        
        inputs = {
            "equipmentData",  // Equipment data
            "environment",    // Environment data
            "characterData",  // Character data
            "equipmentState"  // Equipment state
        };
        
        outputs = {
            "equipmentStatus",  // Equipment status
            "equipmentMetrics"  // Equipment performance metrics
        };
    }
};

class EquipmentManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit EquipmentManager(const EquipmentManagerInfo& info = EquipmentManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void addEquipment(Equipment* equipment);
    void removeEquipment(Equipment* equipment);
    void queryEquipment(Equipment* equipment);
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EquipmentManager() = default;     // Default destructor
};

} // namespace hd
