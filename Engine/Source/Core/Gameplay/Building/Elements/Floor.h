/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Floor.h
 * @brief Header file for the Floor class.
 * 
 * ARCHITECTURAL NOTES:
 * - Floor is a class that represents a floor in Hydragon.
 * - It is used to represent any floor in the game world, such as building floors, bridge floors, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other floors, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct FloorInfo : public BuildingElementInfo {
    FloorInfo() {
        nodeType = "Gameplay/Floor";
        
        inputs = {
            "floorType",  // Type of floor (building floor, bridge floor, etc.)
            "floorData",  // Floor data
            "environment", // Environment data
            "characterData", // Character data
            "floorState" // Floor state
        };
        
        outputs = {
            "floorStatus", // Floor status
            "floorMetrics" // Floor performance metrics
        };
    }
};

class Floor : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Floor(const FloorInfo& info = FloorInfo())
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
