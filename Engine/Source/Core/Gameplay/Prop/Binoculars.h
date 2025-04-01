/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Binoculars.h
 * @brief Header file for the Binoculars class.
 * 
 * ARCHITECTURAL NOTES:
 * - Binoculars is a class that represents a pair of binoculars in Hydragon.
 * - It is used to represent any pair of binoculars in the game world, such as telescopes, etc.
 * - It supports interactive features, such as zooming, focusing, and aiming. Also supports two-way messaging with other binoculars, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Prop.h"
#include "DataTable.h"
#include "Lighting.h"

namespace hd {

struct BinocularsInfo : public PropInfo {
    BinocularsInfo() {
        nodeType = "Gameplay/Binoculars";
        
        inputs = {
            "binocularsType",  // Type of binoculars (telescope, etc.)
            "binocularsData",  // Binoculars data
            "environment",     // Environment data
            "characterData",   // Character data
            "binocularsState"  // Binoculars state
        };
        
        outputs = {
            "binocularsStatus",  // Binoculars status
            "binocularsMetrics"  // Binoculars performance metrics
        };
    }
};

class Binoculars : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Binoculars(const BinocularsInfo& info = BinocularsInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void zoom();
    void focus();
    void aim();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Binoculars() = default;     // Default destructor
};

} // namespace hd
