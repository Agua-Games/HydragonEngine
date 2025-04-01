/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LegArmor.h
 * @brief Header file for the LegArmor class.
 * 
 * ARCHITECTURAL NOTES:
 * - LegArmor is a class that represents leg armor in Hydragon.
 * - It is used to represent any leg armor in the game world, such as pants, boots, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other leg armor, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct LegArmorInfo : public ArmorInfo {
    LegArmorInfo() {
        nodeType = "Gameplay/LegArmor";
        
        inputs = {
            "legArmorType",  // Type of leg armor (pants, boots, etc.)
            "legArmorData",  // Leg armor data
            "environment",   // Environment data
            "characterData", // Character data
            "legArmorState"  // Leg armor state
        };
        
        outputs = {
            "legArmorStatus",  // Leg armor status
            "legArmorMetrics"  // Leg armor performance metrics
        };
    }
};

class LegArmor : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit LegArmor(const LegArmorInfo& info = LegArmorInfo())
        : Armor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processLegArmor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LegArmor() = default;     // Default destructor
};

} // namespace hd
