/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TorsoArmor.h
 * @brief Header file for the TorsoArmor class.
 * 
 * ARCHITECTURAL NOTES:
 * - TorsoArmor is a class that represents torso armor in Hydragon.
 * - It is used to represent any torso armor in the game world, such as vests, jackets, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other torso armor, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct TorsoArmorInfo : public ArmorInfo {
    TorsoArmorInfo() {
        nodeType = "Gameplay/TorsoArmor";
        
        inputs = {
            "torsoArmorType",  // Type of torso armor (vest, jacket, etc.)
            "torsoArmorData",  // Torso armor data
            "environment",     // Environment data
            "characterData",   // Character data
            "torsoArmorState"  // Torso armor state
        };
        
        outputs = {
            "torsoArmorStatus",  // Torso armor status
            "torsoArmorMetrics"  // Torso armor performance metrics
        };
    }
};

class TorsoArmor : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit TorsoArmor(const TorsoArmorInfo& info = TorsoArmorInfo())
        : Armor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processTorsoArmor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TorsoArmor() = default;     // Default destructor
};

} // namespace hd
