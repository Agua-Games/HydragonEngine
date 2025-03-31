/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BattleZone.h
 * @brief Header file for the BattleZone class.
 * 
 * ARCHITECTURAL NOTES:
 * - BattleZone is a class that represents a battle zone in Hydragon.
 * - It is used to represent any battle zone in the game world, such as battlefields, arenas, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other battle zones, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "BattleEvent.h"

namespace hd {

struct BattleZoneInfo : public NodeInfo {
    BattleZoneInfo() {
        nodeType = "Gameplay/BattleZone";
        
        inputs = {
            "zoneType",        // Type of battle zone (battlefield, arena, etc.)
            "zoneData",        // Battle zone data
            "environment",     // Environment data
            "characterData",   // Character data
            "zoneState"        // Battle zone state
        };
        
        outputs = {
            "zoneStatus",      // Battle zone status
            "zoneMetrics"      // Battle zone performance metrics
        };
    }
};

class BattleZone : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BattleZone(const BattleZoneInfo& info = BattleZoneInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    vec3 extents = vec3(0.0f);

    // === Processing ===
    void processNode() override {
 
    }
    void enter();
    void exit();
    void interact();
    void message();
    void processBattleZone();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BattleZone() = default;     // Default destructor
};

}