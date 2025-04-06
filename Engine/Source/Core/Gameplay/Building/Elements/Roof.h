/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Roof.h
 * @brief Header file for the Roof class.
 * 
 * ARCHITECTURAL NOTES:
 * - Roof is a class that represents a roof in Hydragon.
 * - It is used to represent any roof in the game world, such as building roofs, bridge roofs, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other roofs, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct RoofInfo : public BuildingElementInfo {
    RoofInfo() {
        nodeType = "Gameplay/Roof";
        
        inputs = {
            "roofType",  // Type of roof (building roof, bridge roof, etc.)
            "roofData",  // Roof data
            "environment", // Environment data
            "characterData", // Character data
            "roofState" // Roof state
        };
        
        outputs = {
            "roofStatus", // Roof status
            "roofMetrics" // Roof performance metrics
        };
    }
};

class Roof : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Roof(const RoofInfo& info = RoofInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
};

} // namespace hd
