/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Armor.h
 * @brief Header file for the Armor class.
 * 
 * ARCHITECTURAL NOTES:
 * - Armor is a class that represents armor in Hydragon.
 * - It is used to represent any armor in the game world, such as helmets, vests, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other armor, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct ArmorInfo : public EquipmentInfo {
    ArmorInfo() {
        nodeType = "Gameplay/Armor";
        
        inputs = {
            "armorType",      // Type of armor (helmet, vest, etc.)
            "armorData",      // Armor data
            "environment",    // Environment data
            "characterData",  // Character data
            "armorState"      // Armor state
        };
        
        outputs = {
            "armorStatus",    // Armor status
            "armorMetrics"    // Armor performance metrics
        };
    }
};

class Armor : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit Armor(const ArmorInfo& info = ArmorInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float protection = 0.0f;
    float weight = 0.0f;
    float durability = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Armor() = default;     // Default destructor
};

} // namespace hd
