/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WallPanel.h
 * @brief Header file for the WallPanel class.
 * 
 * ARCHITECTURAL NOTES:
 * - WallPanel is a class that represents a wall in Hydragon.
 * - It is used to represent any wall in the game world, such as building walls, bridge walls, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other walls, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct WallPanelInfo : public BuildingElementInfo {
    WallPanelInfo() {
        nodeType = "Gameplay/WallPanel";
        
        inputs = {
            "wallPanelType",  // Type of wall panel (building wall, bridge wall, etc.)
            "wallPanelData",  // Wall data
            "environment", // Environment data
            "characterData", // Character data
            "wallPanelState" // Wall state
        };
        
        outputs = {
            "wallPanelStatus", // Wall status
            "wallPanelMetrics" // Wall performance metrics
        };
    }
};

class WallPanel : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit WallPanel(const WallPanelInfo& info = WallPanelInfo())
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
