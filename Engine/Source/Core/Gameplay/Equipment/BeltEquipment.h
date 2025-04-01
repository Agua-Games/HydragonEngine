/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BeltEquipment.h
 * @brief Header file for the BeltEquipment class.
 * 
 * ARCHITECTURAL NOTES:
 * - BeltEquipment is a class that represents belt equipment in Hydragon.
 * - It is used to represent any belt equipment in the game world, such as belts, vests, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other belt equipment, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct BeltEquipmentInfo : public EquipmentInfo {
    BeltEquipmentInfo() {
        nodeType = "Gameplay/BeltEquipment";
        
        inputs = {
            "beltEquipmentType",  // Type of belt equipment (belt, vest, etc.)
            "beltEquipmentData",  // Belt equipment data
            "environment",        // Environment data
            "characterData",      // Character data
            "beltEquipmentState"  // Belt equipment state
        };
        
        outputs = {
            "beltEquipmentStatus",  // Belt equipment status
            "beltEquipmentMetrics"  // Belt equipment performance metrics
        };
    }
};

class BeltEquipment : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit BeltEquipment(const BeltEquipmentInfo& info = BeltEquipmentInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processBeltEquipment();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BeltEquipment() = default;     // Default destructor
};

} // namespace hd
