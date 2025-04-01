/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Cape.h
 * @brief Header file for the Cape class.
 * 
 * ARCHITECTURAL NOTES:
 * - Cape is a class that represents a cape in Hydragon.
 * - It is used to represent any cape in the game world, such as capes, cloaks, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other capes, environment, character, etc.
 */
#pragma once
#include "Armor.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct CapeInfo : public ArmorInfo {
    CapeInfo() {
        nodeType = "Gameplay/Cape";
        
        inputs = {
            "capeType",  // Type of cape (cape, cloak, etc.)
            "capeData",  // Cape data
            "environment", // Environment data
            "characterData", // Character data
            "capeState" // Cape state
        };
        
        outputs = {
            "capeStatus", // Cape status
            "capeMetrics" // Cape performance metrics
        };
    }
};

class Cape : public Armor {
public:
    // === Allocation, Initialization, Loading ===
    explicit Cape(const CapeInfo& info = CapeInfo())
        : Armor(info) {}
    initialize() override; // Initialize the cape object. This method is called once when the object is created. It is responsible for setting up the initial state and any necessary resources.
    load() override; // Load the cape object. This method is called once when the object is loaded into the game world. It is responsible for loading any necessary assets, such as models, textures, etc.

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processCape();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Cape() = default;     // Default destructor
};

} // namespace hd
