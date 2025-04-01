/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LegGarment.h
 * @brief Header file for the LegGarment class.
 * 
 * ARCHITECTURAL NOTES:
 * - LegGarment is a class that represents leg garment in Hydragon.
 * - It is used to represent any leg garment in the game world, such as pants, shorts, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other leg garment, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct LegGarmentInfo : public ArmorInfo {
    LegGarmentInfo() {
        nodeType = "Gameplay/LegGarment";
        
        inputs = {
            "legGarmentType",  // Type of leg garment (pants, shorts, etc.)
            "legGarmentData",  // Leg garment data
            "environment",     // Environment data
            "characterData",   // Character data
            "legGarmentState"  // Leg garment state
        };
        
        outputs = {
            "legGarmentStatus",  // Leg garment status
            "legGarmentMetrics"  // Leg garment performance metrics
        };
    }
};

class LegGarment : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit LegGarment(const LegGarmentInfo& info = LegGarmentInfo())
        : Armor(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processLegGarment();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LegGarment() = default;     // Default destructor
};

} // namespace hd
