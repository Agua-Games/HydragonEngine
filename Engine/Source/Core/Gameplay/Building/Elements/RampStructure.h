/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RampStructure.h
 * @brief Header file for the RampStructure class.
 * 
 * ARCHITECTURAL NOTES:
 * - RampStructure is a class that represents a ramp structure in Hydragon.
 * - It is used to represent any ramp structure in the game world, such as building ramps, bridge ramps, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other ramp structures, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {
    
struct RampStructureInfo : public BuildingElementInfo {
    RampStructureInfo() {
        nodeType = "Gameplay/RampStructure";
        
        inputs = {
            "rampStructureType",  // Type of ramp structure (building ramp, bridge ramp, etc.)
            "rampStructureData",  // Ramp structure data
            "environment",        // Environment data
            "characterData",      // Character data
            "rampStructureState"  // Ramp structure state
        };
        
        outputs = {
            "rampStructureStatus",  // Ramp structure status
            "rampStructureMetrics"  // Ramp structure performance metrics
        };
    }
};

class RampStructure : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit RampStructure(const RampStructureInfo& info = RampStructureInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
    void interact();     // Interact with the ramp structure (e.g., climb up, climb down, etc.)
};

} // namespace hd
