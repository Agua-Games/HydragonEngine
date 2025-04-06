/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Piping.h
 * @brief Header file for the Piping class.
 * 
 * ARCHITECTURAL NOTES:
 * - Piping is a class that represents a piping in Hydragon.
 * - It is used to represent any piping in the game world, such as building piping, bridge piping, etc.
 *  * - It leverages spline curves with procedural point projection/raycast for placement, crawling over walls, etc. 
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other piping, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct PipingInfo : public BuildingElementInfo {
    PipingInfo() {
        nodeType = "Gameplay/Piping";
        
        inputs = {
            "pipingType",  // Type of piping (building piping, bridge piping, etc.)
            "pipingData",  // Piping data
            "environment", // Environment data
            "characterData", // Character data
            "pipingState" // Piping state
        };
        
        outputs = {
            "pipingStatus", // Piping status
            "pipingMetrics" // Piping performance metrics
        };
    }
};

class Piping : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Piping(const PipingInfo& info = PipingInfo())
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
