/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceColony.h
 * @brief Header file for the SpaceColony class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceColony is a class that represents a space colony in Hydragon.
 * - It is used to represent any space colony in the game world, such as space colonies, space settlements, etc.
 * - It supports interactive features, such as colonizing, terraforming, and trading. Also supports two-way messaging with other space colonies, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Settlement.h"
#include "DataTable.h"

namespace hd {
    
struct SpaceColonyInfo : public SettlementInfo {
    SpaceColonyInfo() {
        nodeType = "Gameplay/SpaceColony";
        
        inputs = {
            "spaceColonyType",  // Type of space colony (space colony, space settlement, etc.)
            "spaceColonyData",  // Space colony data
            "environment",      // Environment data
            "characterData",    // Character data
            "spaceColonyState"  // Space colony state
        };
        
        outputs = {
            "spaceColonyStatus",    // Space colony status
            "spaceColonyMetrics"    // Space colony performance metrics
        };
    }
};

class SpaceColony : public Settlement {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpaceColony(const SpaceColonyInfo& info = SpaceColonyInfo())
        : Settlement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void colonize();
    void terraform();
    void trade();
};

} // namespace hd
