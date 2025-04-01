/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Bridge.h
 * @brief Header file for the Bridge class.
 * 
 * ARCHITECTURAL NOTES:
 * - Bridge is a class that represents a bridge in Hydragon.
 * - It is used to represent any bridge in the game world, such as bridges, viaducts, etc.
 * - It supports interactive features, such as crossing, parking, and interacting with. Also supports two-way messaging with other bridges, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"

namespace hd {

struct BridgeInfo : public BuildingInfo {
    BridgeInfo() {
        nodeType = "Gameplay/Bridge";
        
        inputs = {
            "bridgeType",      // Type of bridge (bridge, viaduct, etc.)
            "bridgeData",      // Bridge data
            "environment",     // Environment data
            "characterData",   // Character data
            "bridgeState"      // Bridge state
        };
        
        outputs = {
            "bridgeStatus",    // Bridge status
            "bridgeMetrics"    // Bridge performance metrics
        };
    }
};

class Bridge : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Bridge(const BridgeInfo& info = BridgeInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processBridge();
    void processNode() override {
        processBridge(); 
    }
    void cross();
    void park();
    void interact();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Bridge() = default;     // Default destructor
};

} // namespace hd
