/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GameplayData.h
 * @brief Header file for the GameplayData class.
 * 
 * ARCHITECTURAL NOTES:
 * - GameplayData is a class that represents gameplay data in Hydragon.
 * - It is used to represent any gameplay data in the game world, such as player stats, enemy health, etc.
 * - It supports interactive features, such as modifying, updating, and accessing. Also supports two-way messaging with other gameplay data, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "GameplayData.h"
#include "DataTable.h"

namespace hd {

struct GameplayDataInfo : public NodeInfo {
    GameplayDataInfo() {
        nodeType = "Gameplay/GameplayData";
        inputs = {
            "data",          // Gameplay data
            "environment",   // Environment data
            "characterData"  // Character data
        };
        outputs = {
            "dataStatus",    // Gameplay data status
            "dataMetrics"    // Gameplay data performance metrics
        };
    }
};

class GameplayData : public Node {
public:
    // === Structure Definitions ===
    struct Settings {
        // Settings for gameplay
    };

    struct GlobalFlags {
        // Global flags for gameplay
    };

    struct LocalFlags {
        // Local flags for gameplay. Uses value pairs to set the locality/scope
    };

    struct Stats {
        // Stats for gameplay
    };

    struct Mode {
        // Mode for gameplay
    };

    struct State {
        // State for gameplay
    };

    struct Mode {
        // Mode for gameplay
    };

    // === Allocation, Initialization, Loading === 
    explicit GameplayData(const GameplayDataInfo& info = GameplayDataInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();
    void checkGlobalFlags();    // Check global flags for gameplay. Uses value pairs to set the locality/scope.
    void setGlobalFlags();      // Set global flags for gameplay. Uses value pairs to set the locality/scope.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GameplayData() = default;     // Default destructor
};

} // namespace hd
