/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SkylightPanel.h
 * @brief Header file for the SkylightPanel class.
 * 
 * ARCHITECTURAL NOTES:
 * - SkylightPanel is a class that represents a skylight in Hydragon.
 * - It is used to represent any skylight in the game world, such as building skylights, bridge skylights, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other skylights, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct SkylightPanelInfo : public BuildingElementInfo {
    SkylightPanelInfo() {
        nodeType = "Gameplay/SkylightPanel";
        
        inputs = {
            "skylightPanelType",  // Type of skylight panel (building skylight, bridge skylight, etc.)
            "skylightPanelData",  // Skylight data
            "environment", // Environment data
            "characterData", // Character data
            "skylightPanelState" // Skylight state
        };
        
        outputs = {
            "skylightPanelStatus", // Skylight status
            "skylightPanelMetrics" // Skylight performance metrics
        };
    }
};

class SkylightPanel : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit SkylightPanel(const SkylightPanelInfo& info = SkylightPanelInfo())
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
