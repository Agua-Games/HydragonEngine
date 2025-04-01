/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Squad.h
 * @brief Header file for the Squad class.
 * 
 * ARCHITECTURAL NOTES:
 * - Squad is a class that represents a squad in Hydragon.
 * - It is used to represent any squad in the game world, such as player squads, enemy squads, etc.
 * - It supports interactive features, such as squad management, squad communication, squad objectives, etc. Also supports two-way messaging with other squads, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SquadInfo : public NodeInfo {
    SquadInfo() {
        nodeType = "Gameplay/Squad";
        
        inputs = {
            "squadData",        // Squad data
            "environment",      // Environment data
            "characterData",    // Character data
            "squadState"        // Squad state
        };
        
        outputs = {
            "squadStatus",      // Squad status
            "squadMetrics"      // Squad performance metrics
        };
    }
};

class Squad : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Squad(const SquadInfo& info = SquadInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processSquad();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Squad() = default;     // Default destructor
};

} // namespace hd
