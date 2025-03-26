/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WatchTower.h
 * @brief Header file for the WatchTower class.
 * 
 * ARCHITECTURAL NOTES:
 * - WatchTower is a class that represents a watchtower in Hydragon.
 * - It is used to represent any watchtower in the game world, such as guard towers, observation towers, etc.
 * - It supports interactive features, such as watching, guarding, and signaling. Also supports two-way messaging with other watchtowers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct WatchTowerInfo : public NodeInfo {
    WatchTowerInfo() {
        nodeType = "Gameplay/WatchTower";
        
        inputs = {
            "watchTowerType",  // Type of watchtower (guard tower, observation tower, etc.)
            "watchTowerData",  // Watchtower data
            "environment",     // Environment data
            "characterData",   // Character data
            "watchTowerState"  // Watchtower state
        };
        
        outputs = {
            "watchTowerStatus",  // Watchtower status
            "watchTowerMetrics"  // Watchtower performance metrics
        };
    }
};

class WatchTower : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit WatchTower(const WatchTowerInfo& info = WatchTowerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WatchTower() = default;     // Default destructor
};

} // namespace hd
