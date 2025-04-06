/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Cornice.h
 * @brief Header file for the Cornice class.
 * 
 * ARCHITECTURAL NOTES:
 * - Cornice is a class that represents a cornice in Hydragon.
 * - It is used to represent any cornice in the game world, such as building cornices, bridge cornices, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other cornices, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct CorniceInfo : public BuildingElementInfo {
    CorniceInfo() {
        nodeType = "Gameplay/Cornice";
        
        inputs = {
            "corniceType",  // Type of cornice (building cornice, bridge cornice, etc.)
            "corniceData",  // Cornice data
            "environment",  // Environment data
            "characterData", // Character data
            "corniceState" // Cornice state
        };
        
        outputs = {
            "corniceStatus", // Cornice status
            "corniceMetrics" // Cornice performance metrics
        };
    }
};

class Cornice : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Cornice(const CorniceInfo& info = CorniceInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
    void interact();     // Interact with the cornice (e.g., climb up, climb down, etc.)
};

} // namespace hd
