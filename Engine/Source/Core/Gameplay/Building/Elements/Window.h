/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Window.h
 * @brief Header file for the Window class.
 * 
 * ARCHITECTURAL NOTES:
 * - Window is a class that represents a window in Hydragon.
 * - It is used to represent any window in the game world, such as building windows, bridge windows, etc.
 * - It supports interactive features, such as opening, closing, and locking. Also supports two-way messaging with other windows, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct WindowInfo : public BuildingElementInfo {
    WindowInfo() {
        nodeType = "Gameplay/Window";
        
        inputs = {
            "windowType",  // Type of window (building window, bridge window, etc.)
            "windowData",  // Window data
            "environment", // Environment data
            "characterData", // Character data
            "windowState" // Window state
        };
        
        outputs = {
            "windowStatus", // Window status
            "windowMetrics" // Window performance metrics
        };
    }
};

class Window : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Window(const WindowInfo& info = WindowInfo())
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
