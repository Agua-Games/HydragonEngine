/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Hammer.h
 * @brief Header file for the Hammer class.
 * 
 * ARCHITECTURAL NOTES:
 * - Hammer is a class that represents a hammer in Hydragon.
 * - It is used to represent any hammer in the game world, such as sledgehammers, hammers, etc.
 * - It supports interactive features, such as hammering, breaking, and smashing. Also supports two-way messaging with other hammers, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Prop.h"
#include "DataTable.h"

namespace hd {

struct HammerInfo : public PropInfo {
    HammerInfo() {
        nodeType = "Gameplay/Hammer";
        
        inputs = {
            "hammerType",  // Type of hammer (sledgehammer, hammer, etc.)
            "hammerData",  // Hammer data
            "environment", // Environment data
            "characterData", // Character data
            "hammerState" // Hammer state
        };
        
        outputs = {
            "hammerStatus", // Hammer status
            "hammerMetrics" // Hammer performance metrics
        };
    }
};

class Hammer : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Hammer(const HammerInfo& info = HammerInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void hammer();
    void breakObject();
    void smash();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Hammer() = default;     // Default destructor
};

} // namespace hd
