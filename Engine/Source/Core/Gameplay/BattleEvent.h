/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BattleEvent.h
 * @brief Header file for the BattleEvent class.
 * 
 * ARCHITECTURAL NOTES:
 * - BattleEvent is a class that represents a battle event in Hydragon.
 * - It is used to represent any battle event in the game world, such as player attacks, enemy attacks, etc.
 * - It supports interactive features, such as triggering events and responding to events. Also supports two-way messaging with other battle events, environment, character, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct BattleEventInfo : public NodeInfo {
    BattleEventInfo() {
        nodeType = "Gameplay/BattleEvent";
        
        inputs = {
            "eventType",       // Type of battle event (player attack, enemy attack, etc.)
            "eventData",       // Battle event data
            "environment",     // Environment data
            "characterData",   // Character data
            "eventState"       // Battle event state
        };
        
        outputs = {
            "eventStatus",     // Battle event status
            "eventMetrics"     // Battle event performance metrics
        };
    }
};

class BattleEvent : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BattleEvent(const BattleEventInfo& info = BattleEventInfo())
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
    ~BattleEvent() = default;     // Default destructor
};

} // namespace hd
