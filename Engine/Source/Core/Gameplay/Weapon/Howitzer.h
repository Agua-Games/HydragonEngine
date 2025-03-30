/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Howitzer.h
 * @brief Header file for the Howitzer class.
 * 
 * ARCHITECTURAL NOTES:
 * - Howitzer is a class that represents a howitzer in Hydragon.
 * - It is used to represent any howitzer in the game world, such as mortars, howitzers, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other howitzers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct HowitzerInfo : public WeaponInfo {
    HowitzerInfo() {
        nodeType = "Gameplay/Howitzer";
        
        inputs = {
            "howitzerType",  // Type of howitzer (mortar, howitzer, etc.)
            "howitzerData",  // Howitzer data
            "environment", // Environment data
            "characterData", // Character data
            "howitzerState" // Howitzer state
        };
        
        outputs = {
            "howitzerStatus", // Howitzer status
            "howitzerMetrics" // Howitzer performance metrics
        };
    }
};

class Howitzer : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Howitzer(const HowitzerInfo& info = HowitzerInfo())
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
    ~Howitzer() = default;     // Default destructor
};

} // namespace hd