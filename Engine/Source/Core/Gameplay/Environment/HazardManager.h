/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HazardManager.h
 * @brief Header file for the HazardManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - HazardManager is a singleton class that manages the hazards in the game.
 * - It is responsible for managing the hazards in the game.
 * - It supports interactive features, such as detecting and responding to hazards. Also supports two-way messaging with other hazards, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct HazardManagerInfo : public NodeInfo {
    HazardManagerInfo() {
        nodeType = "Gameplay/HazardManager";
        
        inputs = {
            "hazardData",      // Hazard data
            "environment",     // Environment data
            "characterData",   // Character data
            "hazardState"      // Hazard state
        };
        
        outputs = {
            "hazardStatus",    // Hazard status
            "hazardMetrics"    // Hazard performance metrics
        };
    }
};

class HazardManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit HazardManager(const HazardManagerInfo& info = HazardManagerInfo())
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
    ~HazardManager() = default;     // Default destructor
};

} // namespace hd
