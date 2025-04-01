/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FootGarment.h
 * @brief Header file for the FootGarment class.
 * 
 * ARCHITECTURAL NOTES:
 * - FootGarment is a class that represents feet garment in Hydragon.
 * - It is used to represent any feet garment in the game world, such as boots, shoes, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other feet garment, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct FootGarmentInfo : public ArmorInfo {
    FootGarmentInfo() {
        nodeType = "Gameplay/FootGarment";
        
        inputs = {
            "footGarmentType",  // Type of foot garment (boots, shoes, etc.)
            "footGarmentData",  // Feet garment data
            "environment",      // Environment data
            "characterData",    // Character data
            "footGarmentState"  // Feet garment state
        };
        
        outputs = {
            "footGarmentStatus",  // Foot garment status
            "footGarmentMetrics"  // Foot garment performance metrics
        };
    }
};

class FootGarment : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit FootGarment(const FootGarmentInfo& info = FootGarmentInfo())
        : Armor(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processFootGarment();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FootGarment() = default;     // Default destructor
};

} // namespace hd
