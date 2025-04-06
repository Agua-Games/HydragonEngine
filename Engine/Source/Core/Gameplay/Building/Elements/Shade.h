/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Shade.h
 * @brief Header file for the Shade class.
 * 
 * ARCHITECTURAL NOTES:
 * - Shade is a class that represents a shade in Hydragon.
 * - It is used to represent any shade in the game world, such as building shades, bridge shades, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other shades, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct ShadeInfo : public BuildingElementInfo {
    ShadeInfo() {
        nodeType = "Gameplay/Shade";
        
        inputs = {
            "shadeType",  // Type of shade (building shade, bridge shade, etc.)
            "shadeData",  // Shade data
            "environment", // Environment data
            "characterData", // Character data
            "shadeState" // Shade state
        };
        
        outputs = {
            "shadeStatus", // Shade status
            "shadeMetrics" // Shade performance metrics
        };
    }
};

class Shade : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Shade(const ShadeInfo& info = ShadeInfo())
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
