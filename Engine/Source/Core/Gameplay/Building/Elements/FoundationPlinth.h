/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FoundationPlinth.h
 * @brief Header file for the FoundationPlinth class.
 * 
 * ARCHITECTURAL NOTES:
 * - FoundationPlinth is a class that represents the foundation and plinth in Hydragon.
 * - It is used to represent any foundation and plinth in the game world, such as building foundations, bridge foundations, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other foundations, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct FoundationPlinthInfo : public BuildingElementInfo {
    FoundationPlinthInfo() {
        nodeType = "Gameplay/FoundationPlinth";
        
        inputs = {
            "foundationPlinthType",  // Type of foundation (building foundation, bridge foundation, etc.)
            "foundationPlinthData",  // Foundation data
            "environment",     // Environment data
            "characterData",   // Character data
            "foundationPlinthState"  // Foundation state
        };
        
        outputs = {
            "foundationPlinthStatus",  // Foundation status
            "foundationPlinthMetrics"  // Foundation performance metrics
        };
    }
};

class FoundationPlinth : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit FoundationPlinth(const FoundationPlinthInfo& info = FoundationPlinthInfo())
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
