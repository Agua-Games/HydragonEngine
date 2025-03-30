/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AutoNavigator.h
 * @brief Header file for the AutoNavigator class.
 * 
 * ARCHITECTURAL NOTES:
 * - AutoNavigator is a class that represents an auto navigator in Hydragon.
 * - It is used to represent any auto navigation systems, such as autonomous navigation, auto-pilot, pathfinding, etc. (e.g., AI navigation)
 * - It supports interactive features, such as navigation, monitoring, and updating. Also supports two-way messaging with other auto navigation systems, environment, character, etc.
 */
#pragma once
#include <vector>
#include "Node.h"
#include "DataTable.h"
#include "EnvironmentManager.h"

namespace hd {

struct AutoNavigatorInfo : public NodeInfo {
    AutoNavigatorInfo() {
        nodeType = "Gameplay/AutoNavigator";
        
        inputs = {
            "autoNavigatorData",  // Auto navigator data
            "environment",        // Environment data
            "characterData",      // Character data
            "autoNavigatorState"  // Auto navigator state
        };
        
        outputs = {
            "autoNavigatorStatus", // Auto navigator status
            "autoNavigatorMetrics" // Auto navigator performance metrics
        };
    }
};

class AutoNavigator : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit AutoNavigator(const AutoNavigatorInfo& info = AutoNavigatorInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    AutoNavigatorInfo autoNavigatorInfo = {};  // Initialize auto navigator info struct with default values

    // === Processing ===
    void processNode() override { }
    void navigate();
    void monitor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AutoNavigator() = default;     // Default destructor
};

} // namespace hd
