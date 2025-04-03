/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file City.h
 * @brief Header file for the City class.
 * 
 * ARCHITECTURAL NOTES:
 * - City is a class that represents a city in Hydragon.
 * - It is used to represent any city in the game world, such as cities, towns, villages, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other cities, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Settlement.h"
#include "DataTable.h"

namespace hd {
    
struct CityInfo : public SettlementInfo {
    CityInfo() {
        nodeType = "Gameplay/City";
        
        inputs = {
            "cityType",        // Type of city (city, town, village, etc.)
            "cityData",        // City data
            "environment",     // Environment data
            "characterData",   // Character data
            "cityState"        // City state
        };
        
        outputs = {
            "cityStatus",      // City status
            "cityMetrics"      // City performance metrics
        };
    }
};

class City : public Settlement {
public:
    // === Allocation, Initialization, Loading ===
    explicit City(const CityInfo& info = CityInfo())
        : Settlement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
};

} // namespace hd
