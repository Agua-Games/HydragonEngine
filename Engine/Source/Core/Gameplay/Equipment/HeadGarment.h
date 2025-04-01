/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HeadGarment.h
 * @brief Header file for the HeadGarment class.
 * 
 * ARCHITECTURAL NOTES:
 * - HeadGarment is a class tHeadGarment represents a head garment in Hydragon.
 * - It is used to represent any head garment in the game world, such as a hat, a cap, a hood, etc,
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other HeadGarments, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct HeadGarmentInfo : public EquipmentInfo {
    HeadGarmentInfo() {
        nodeType = "Gameplay/HeadGarment";
        
        inputs = {
            "HeadGarmentType",  // Type of HeadGarment (HeadGarment, headgear, etc.)
            "HeadGarmentData",  // HeadGarment data
            "environment", // Environment data
            "characterData", // Character data
            "HeadGarmentState" // HeadGarment state
        };
        
        outputs = {
            "HeadGarmentStatus", // HeadGarment status
            "HeadGarmentMetrics" // HeadGarment performance metrics
        };
    }
};

class HeadGarment : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit HeadGarment(const HeadGarmentInfo& info = HeadGarmentInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processHeadGarment();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~HeadGarment() = default;     // Default destructor
};

} // namespace hd
