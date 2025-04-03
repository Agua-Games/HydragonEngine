/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Village.h
 * @brief Header file for the Village class.
 * 
 * ARCHITECTURAL NOTES:
 * - Village is a class that represents a village in Hydragon.
 * - It is used to represent any village in the game world, such as hamlets, towns, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other villages, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Settlement.h"
#include "DataTable.h"

namespace hd {
    
struct VillageInfo : public SettlementInfo {
    VillageInfo() {
        nodeType = "Gameplay/Village";
        
        inputs = {
            "villageType",      // Type of village (hamlet, town, etc.)
            "villageData",      // Village data
            "environment",      // Environment data
            "characterData",    // Character data
            "villageState"      // Village state
        };
        
        outputs = {
            "villageStatus",    // Village status
            "villageMetrics"    // Village performance metrics
        };
    }
};

class Village : public Settlement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Village(const VillageInfo& info = VillageInfo())
        : Settlement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
};

}