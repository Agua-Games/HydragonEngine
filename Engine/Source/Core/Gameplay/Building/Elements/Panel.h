/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Panel.h
 * @brief Header file for the Panel class.
 * 
 * ARCHITECTURAL NOTES:
 * - Panel is a class that represents a control panel in Hydragon.
 * - It is used to represent any control panel or container device with control panel in the game world, such as air-conditioners, electric panels, 
 * elevator control panels, spaceship control panels, hangar control panels, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other panels, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct PanelInfo : public BuildingElementInfo {
    PanelInfo() {
        nodeType = "Gameplay/Panel";
        
        inputs = {
            "panelType",  // Type of panel (air-conditioner, electric panel, elevator control panel, etc.)
            "panelData",  // Panel data
            "environment", // Environment data
            "characterData", // Character data
            "panelState" // Panel state
        };
        
        outputs = {
            "panelStatus", // Panel status
            "panelMetrics" // Panel performance metrics
        };
    }
};

class Panel : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Panel(const PanelInfo& info = PanelInfo())
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
