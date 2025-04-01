/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Glasses.h
 * @brief Header file for the Glasses class.
 * 
 * ARCHITECTURAL NOTES:
 * - Glasses is a class that represents glasses in Hydragon.
 * - It is used to represent any glasses in the game world, such as sunglasses, eyeglasses, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other glasses, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct GlassesInfo : public EquipmentInfo {
    GlassesInfo() {
        nodeType = "Gameplay/Glasses";
        
        inputs = {
            "glassesType",  // Type of glasses (sunglasses, eyeglasses, etc.)
            "glassesData",  // Glasses data
            "environment", // Environment data
            "characterData", // Character data
            "glassesState" // Glasses state
        };
        
        outputs = {
            "glassesStatus", // Glasses status
            "glassesMetrics" // Glasses performance metrics
        };
    }
};

class Glasses : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit Glasses(const GlassesInfo& info = GlassesInfo())
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
    ~Glasses() = default;     // Default destructor
};

} // namespace hd
