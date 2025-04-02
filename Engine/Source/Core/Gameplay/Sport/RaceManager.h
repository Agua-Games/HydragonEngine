/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RaceManager.h
 * @brief Header file for the RaceManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - RaceManager is a singleton class that manages the races in the game.
 * - It is responsible for managing the races in the game.
 * - It supports interactive features, such as starting, ending, and managing races. Also supports two-way messaging with other races, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"

namespace hd {

struct RaceManagerInfo : public NodeInfo {
    RaceManagerInfo() {
        nodeType = "Gameplay/RaceManager";
        
        inputs = {
            "raceData",        // Race data
            "environment",     // Environment data
            "characterData",   // Character data
            "raceState"        // Race state
        };
        
        outputs = {
            "raceStatus",      // Race status
            "raceMetrics"      // Race performance metrics
        };
    }
};

class RaceManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RaceManager(const RaceManagerInfo& info = RaceManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RaceManager() = default;     // Default destructor
};

} // namespace hd

