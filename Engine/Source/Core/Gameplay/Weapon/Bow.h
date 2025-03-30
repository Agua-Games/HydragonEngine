/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Bow.h
 * @brief Header file for the Bow class.
 * 
 * ARCHITECTURAL NOTES:
 * - Bow is a class that represents a bow in Hydragon.
 * - It is used to represent any bow in the game world, such as longbows, shortbows, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other bows, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct BowInfo : public WeaponInfo {
    BowInfo() {
        nodeType = "Gameplay/Bow";
        
        inputs = {
            "bowType",  // Type of bow (longbow, shortbow, etc.)
            "bowData",  // Bow data
            "environment", // Environment data
            "characterData", // Character data
            "bowState" // Bow state
        };
        
        outputs = {
            "bowStatus", // Bow status
            "bowMetrics" // Bow performance metrics
        };
    }
};

class Bow : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Bow(const BowInfo& info = BowInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void fire();
    void reload();
    void aim();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Bow() = default;     // Default destructor
};

} // namespace hd