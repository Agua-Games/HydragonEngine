/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GameplayEvent.h
 * @brief Header file for the GameplayEvent class.
 * 
 * ARCHITECTURAL NOTES:
 * - GameplayEvent is a class that represents a gameplay event in Hydragon.
 * - It is used to represent any gameplay event in the game world, such as player deaths, enemy attacks, etc.
 * - It supports interactive features, such as triggering events and responding to events. Also supports two-way messaging with other gameplay events, environment, character, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct GameplayEventInfo : public NodeInfo {
    GameplayEventInfo() {
        nodeType = "Gameplay/GameplayEvent";
        
        inputs = {
            "eventType",       // Type of gameplay event (player death, enemy attack, etc.)
            "eventData",       // Gameplay event data
            "environment",     // Environment data
            "characterData",   // Character data
            "eventState"       // Gameplay event state
        };
        
        outputs = {
            "eventStatus",     // Gameplay event status
            "eventMetrics"     // Gameplay event performance metrics
        };
    }
};

class GameplayEvent : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit GameplayEvent(const GameplayEventInfo& info = GameplayEventInfo())
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
    ~GameplayEvent() = default;     // Default destructor
};

} // namespace hd
