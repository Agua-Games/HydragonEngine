/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Mask.h
 * @brief Header file for the Mask class.
 * 
 * ARCHITECTURAL NOTES:
 * - Mask is a class that represents a mask in Hydragon.
 * - It is used to represent any mask in the game world, such as masks, helmets, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other masks, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct MaskInfo : public ArmorInfo {
    MaskInfo() {
        nodeType = "Gameplay/Mask";
        
        inputs = {
            "maskType",  // Type of mask (mask, helmet, etc.)
            "maskData",  // Mask data
            "environment", // Environment data
            "characterData", // Character data
            "maskState" // Mask state
        };
        
        outputs = {
            "maskStatus", // Mask status
            "maskMetrics" // Mask performance metrics
        };
    }
};

class Mask : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit Mask(const MaskInfo& info = MaskInfo())
        : Armor(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processMask();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Mask() = default;     // Default destructor
};

} // namespace hd
