/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Vest.h
 * @brief Header file for the Vest class.
 * 
 * ARCHITECTURAL NOTES:
 * - Vest is a class that represents a vest in Hydragon.
 * - It is used to represent any vest in the game world, such as vests, jackets, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other vests, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct VestInfo : public ArmorInfo {
    VestInfo() {
        nodeType = "Gameplay/Vest";
        
        inputs = {
            "vestType",  // Type of vest (vest, jacket, etc.)
            "vestData",  // Vest data
            "environment", // Environment data
            "characterData", // Character data
            "vestState" // Vest state
        };
        
        outputs = {
            "vestStatus", // Vest status
            "vestMetrics" // Vest performance metrics
        };
    }
};

class Vest : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit Vest(const VestInfo& info = VestInfo())
        : Armor(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processVest();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Vest() = default;     // Default destructor
};

} // namespace hd
