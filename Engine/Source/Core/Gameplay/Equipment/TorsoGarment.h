/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TorsoGarment.h
 * @brief Header file for the TorsoGarment class.
 * 
 * ARCHITECTURAL NOTES:
 * - TorsoGarment is a class that represents torso garment in Hydragon.
 * - It is used to represent any torso garment in the game world, such as shirts, jackets, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other torso garment, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct TorsoGarmentInfo : public ArmorInfo {
    TorsoGarmentInfo() {
        nodeType = "Gameplay/TorsoGarment";
        
        inputs = {
            "torsoGarmentType",  // Type of torso garment (shirt, jacket, etc.)
            "torsoGarmentData",  // Torso garment data
            "environment",       // Environment data
            "characterData",     // Character data
            "torsoGarmentState"  // Torso garment state
        };
        
        outputs = {
            "torsoGarmentStatus",  // Torso garment status
            "torsoGarmentMetrics"  // Torso garment performance metrics
        };
    }
};

class TorsoGarment : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit TorsoGarment(const TorsoGarmentInfo& info = TorsoGarmentInfo())
        : Armor(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processTorsoGarment();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TorsoGarment() = default;     // Default destructor
};

} // namespace hd
