/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Glove.h
 * @brief Header file for the Glove class.
 * 
 * ARCHITECTURAL NOTES:
 * - Glove is a class that represents a glove in Hydragon.
 * - It is used to represent any glove in the game world, such as gloves, gauntlets, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other gloves, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct GloveInfo : public ArmorInfo {
    GloveInfo() {
        nodeType = "Gameplay/Glove";
        
        inputs = {
            "gloveType",  // Type of glove (gloves, gauntlets, etc.)
            "gloveData",  // Glove data
            "environment", // Environment data
            "characterData", // Character data
            "gloveState" // Glove state
        };
        
        outputs = {
            "gloveStatus", // Glove status
            "gloveMetrics" // Glove performance metrics
        };
    }
};

class Glove : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit Glove(const GloveInfo& info = GloveInfo())
        : Armor(info) {}
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
    ~Glove() = default;     // Default destructor
};

} // namespace hd
