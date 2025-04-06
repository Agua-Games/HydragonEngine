/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Balcony.h
 * @brief Header file for the Balcony class.
 * 
 * ARCHITECTURAL NOTES:
 * - Balcony is a class that represents a balcony in Hydragon.
 * - It is used to represent any balcony in the game world, such as building balconies, bridge balconies, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other balconies, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct BalconyInfo : public BuildingElementInfo {
    BalconyInfo() {
        nodeType = "Gameplay/Balcony";
        
        inputs = {
            "balconyType",  // Type of balcony (building balcony, bridge balcony, etc.)
            "balconyData",  // Balcony data
            "environment", // Environment data
            "characterData", // Character data
            "balconyState" // Balcony state
        };
        
        outputs = {
            "balconyStatus", // Balcony status
            "balconyMetrics" // Balcony performance metrics
        };
    }
};

class Balcony : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Balcony(const BalconyInfo& info = BalconyInfo())
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
