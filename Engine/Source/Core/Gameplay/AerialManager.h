/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AerialManager.h
 * @brief Header file for the AerialManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - AerialManager is a singleton class that manages the aerial characters and vehicles in the game.
 * - It is responsible for managing the aerial characters and vehicles in the game.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other aerial characters and vehicles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct AerialManagerInfo : public NodeInfo {
    AerialManagerInfo() {
        nodeType = "Gameplay/AerialManager";
        
        inputs = {
            "aerialData",        // Aerial data
            "environment",       // Environment data
            "characterData",     // Character data
            "aerialState"        // Aerial state
        };
        
        outputs = {
            "aerialStatus",      // Aerial status
            "aerialMetrics"      // Aerial performance metrics
        };
    }
};

class AerialManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AerialManager(const AerialManagerInfo& info = AerialManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AerialManager() = default;     // Default destructor
};

} // namespace hd