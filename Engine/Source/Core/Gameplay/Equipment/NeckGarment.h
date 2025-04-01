/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NeckGarment.h
 * @brief Header file for the NeckGarment class.
 * 
 * ARCHITECTURAL NOTES:
 * - NeckGarment is a class that represents a neck garment in Hydragon.
 * - It is used to represent any neck garment in the game world, such as necklaces, collars, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other neck garments, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct NeckGarmentInfo : public EquipmentInfo {
    NeckGarmentInfo() {
        nodeType = "Gameplay/NeckGarment";
        
        inputs = {
            "neckGarmentType",  // Type of neck garment (necklace, collar, etc.)
            "neckGarmentData",  // Neck garment data
            "environment",      // Environment data
            "characterData",    // Character data
            "neckGarmentState"  // Neck garment state
        };
        
        outputs = {
            "neckGarmentStatus",  // Neck garment status
            "neckGarmentMetrics"  // Neck garment performance metrics
        };
    }
};

class NeckGarment : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit NeckGarment(const NeckGarmentInfo& info = NeckGarmentInfo())
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
    ~NeckGarment() = default;     // Default destructor
};

} // namespace hd
