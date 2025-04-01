/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Whip.h
 * @brief Header file for the Whip class.
 * 
 * ARCHITECTURAL NOTES:
 * - Whip is a class that represents a whip in Hydragon.
 * - It is used to represent any whip in the game world, such as whips, ropes, etc.
 * - It supports interactive features, such as whipping, reloading, and aiming. Also supports two-way messaging with other whips, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct WhipInfo : public WeaponInfo {
    WhipInfo() {
        nodeType = "Gameplay/Whip";
        
        inputs = {
            "whipType",  // Type of whip (whip, rope, etc.)
            "whipData",  // Whip data
            "environment", // Environment data
            "characterData", // Character data
            "whipState" // Whip state
        };
        
        outputs = {
            "whipStatus", // Whip status
            "whipMetrics" // Whip performance metrics
        };
    }
};

class Whip : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Whip(const WhipInfo& info = WhipInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Whip() = default;     // Default destructor
};

} // namespace hd
