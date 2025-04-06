/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Railing.h
 * @brief Header file for the Railing class.
 * 
 * ARCHITECTURAL NOTES:
 * - Railing is a class that represents a railing in Hydragon.
 * - It is used to represent any railing in the game world, such as building railings, bridge railings, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other railings, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct RailingInfo : public BuildingElementInfo {
    RailingInfo() {
        nodeType = "Gameplay/Railing";
        
        inputs = {
            "railingType",       // Type of railing (building railing, bridge railing, etc.)
            "railingData",       // Railing data
            "environment",       // Environment data
            "characterData",     // Character data
            "railingState"       // Railing state
        };
        
        outputs = {
            "railingStatus",     // Railing status
            "railingMetrics"     // Railing performance metrics
        };
    }
};

class Railing : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Railing(const RailingInfo& info = RailingInfo())
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
