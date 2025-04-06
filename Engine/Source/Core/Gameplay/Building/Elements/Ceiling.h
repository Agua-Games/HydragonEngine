/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Ceiling.h
 * @brief Header file for the Ceiling class.
 * 
 * ARCHITECTURAL NOTES:
 * - Ceiling is a class that represents a ceiling in Hydragon.
 * - It is used to represent any ceiling in the game world, such as building ceilings, bridge ceilings, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other ceilings, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct CeilingInfo : public BuildingElementInfo {
    CeilingInfo() {
        nodeType = "Gameplay/Ceiling";
        
        inputs = {
            "ceilingType",  // Type of ceiling (building ceiling, bridge ceiling, etc.)
            "ceilingData",  // Ceiling data
            "environment",  // Environment data
            "characterData", // Character data
            "ceilingState" // Ceiling state
        };
        
        outputs = {
            "ceilingStatus", // Ceiling status
            "ceilingMetrics" // Ceiling performance metrics
        };
    }
};

class Ceiling : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Ceiling(const CeilingInfo& info = CeilingInfo())
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
