/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Helmet.h
 * @brief Header file for the Helmet class.
 * 
 * ARCHITECTURAL NOTES:
 * - Helmet is a class that represents a helmet in Hydragon.
 * - It is used to represent any helmet in the game world, such as helmets, headgear, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other helmets, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct HelmetInfo : public ArmorInfo {
    HelmetInfo() {
        nodeType = "Gameplay/Helmet";
        
        inputs = {
            "helmetType",  // Type of helmet (helmet, headgear, etc.)
            "helmetData",  // Helmet data
            "environment", // Environment data
            "characterData", // Character data
            "helmetState" // Helmet state
        };
        
        outputs = {
            "helmetStatus", // Helmet status
            "helmetMetrics" // Helmet performance metrics
        };
    }
};

class Helmet : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit Helmet(const HelmetInfo& info = HelmetInfo())
        : Armor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processHelmet();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Helmet() = default;     // Default destructor
};

} // namespace hd
