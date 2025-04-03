/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LandingPad.h
 * @brief Header file for the LandingPad class.
 * 
 * ARCHITECTURAL NOTES:
 * - LandingPad is a class that represents a landing pad in Hydragon.
 * - It is used to represent any landing pad in the game world, such as helipads, runways, etc.
 * - It supports interactive features, such as landing, taking off, and parking. Also supports two-way messaging with other landing pads, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"

namespace hd {

struct LandingPadInfo : public BuildingInfo {
    LandingPadInfo() {
        nodeType = "Gameplay/LandingPad";
        
        inputs = {
            "landingPadType",  // Type of landing pad (helipad, runway, etc.)
            "landingPadData",  // Landing pad data
            "environment",     // Environment data
            "characterData",   // Character data
            "landingPadState"  // Landing pad state
        };
        
        outputs = {
            "landingPadStatus",  // Landing pad status
            "landingPadMetrics"  // Landing pad performance metrics
        };
    }
};

class LandingPad : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit LandingPad(const LandingPadInfo& info = LandingPadInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void landing();
    void takeOff();
    void parking();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LandingPad() = default;     // Default destructor
};

} // namespace hd
