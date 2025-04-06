/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Doorway.h
 * @brief Header file for the Doorway class.
 * 
 * ARCHITECTURAL NOTES:
 * - Doorway is a class that represents a doorway in Hydragon.
 * - It is used to represent any doorway in the game world, such as building doors, including glass doors, grand entrances, bridge doors, etc.
 * - It supports interactive features, such as opening, closing, and locking. Also supports two-way messaging with other doorways, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct DoorwayInfo : public BuildingElementInfo {
    DoorwayInfo() {
        nodeType = "Gameplay/Doorway";
        
        inputs = {
            "doorwayType",  // Type of doorway (building doorway, bridge doorway, etc.)
            "doorwayData",  // doorway data
            "environment", // Environment data
            "characterData", // Character data
            "doorwayState" // doorway state
        };
        
        outputs = {
            "doorwayStatus", // doorway status
            "doorwayMetrics" // doorway performance metrics
        };
    }
};

class Doorway : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Doorway(const DoorwayInfo& info = DoorwayInfo())
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
