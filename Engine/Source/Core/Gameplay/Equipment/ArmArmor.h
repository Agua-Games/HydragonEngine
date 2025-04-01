/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ArmArmor.h
 * @brief Header file for the ArmArmor class.
 * 
 * ARCHITECTURAL NOTES:
 * - ArmArmor is a class that represents arm armor in Hydragon.
 * - It is used to represent any arm armor in the game world, such as gloves, gauntlets, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other arm armor, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct ArmArmorInfo : public ArmorInfo {
    ArmArmorInfo() {
        nodeType = "Gameplay/ArmArmor";
        
        inputs = {
            "armArmorType",  // Type of arm armor (gloves, gauntlets, etc.)
            "armArmorData",  // Arm armor data
            "environment",   // Environment data
            "characterData", // Character data
            "armArmorState"  // Arm armor state
        };
        
        outputs = {
            "armArmorStatus",  // Arm armor status
            "armArmorMetrics"  // Arm armor performance metrics
        };
    }
};

class ArmArmor : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit ArmArmor(const ArmArmorInfo& info = ArmArmorInfo())
        : Armor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processArmArmor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ArmArmor() = default;     // Default destructor
};

} // namespace hd
