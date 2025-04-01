/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Goggles.h
 * @brief Header file for the Goggles class.
 * 
 * ARCHITECTURAL NOTES:
 * - Goggles is a class that represents goggles in Hydragon.
 * - It is used to represent any goggles in the game world, such as sunglasses, visors, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other goggles, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct GogglesInfo : public EquipmentInfo {
    GogglesInfo() {
        nodeType = "Gameplay/Goggles";
        
        inputs = {
            "gogglesType",  // Type of goggles (sunglasses, visor, etc.)
            "gogglesData",  // Goggles data
            "environment", // Environment data
            "characterData", // Character data
            "gogglesState" // Goggles state
        };
        
        outputs = {
            "gogglesStatus", // Goggles status
            "gogglesMetrics" // Goggles performance metrics
        };
    }
};

class Goggles : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit Goggles(const GogglesInfo& info = GogglesInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void equip();
    void unequip();
    void upgrade();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Goggles() = default;     // Default destructor
};

} // namespace hd
