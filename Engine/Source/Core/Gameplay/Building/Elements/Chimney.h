/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Chimney.h
 * @brief Header file for the Chimney class.
 * 
 * ARCHITECTURAL NOTES:
 * - Chimney is a class that represents a chimney in Hydragon.
 * - It is used to represent any chimney in the game world, such as building chimneys, bridge chimneys, etc.
 * - It leverages spline curves with procedural point projection/raycast for placement, crawling over walls, etc. 
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other chimneys, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"
#include "SplineScatter.h"

namespace hd {

struct ChimneyInfo : public BuildingElementInfo {
    ChimneyInfo() {
        nodeType = "Gameplay/Chimney";
        
        inputs = {
            "chimneyType",      // Type of chimney (building chimney, bridge chimney, etc.)
            "chimneyData",      // Chimney data
            "environment",      // Environment data
            "characterData",    // Character data
            "chimneyState"      // Chimney state
        };
        
        outputs = {
            "chimneyStatus",    // Chimney status
            "chimneyMetrics"    // Chimney performance metrics
        };
    }
};

class Chimney : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Chimney(const ChimneyInfo& info = ChimneyInfo())
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
