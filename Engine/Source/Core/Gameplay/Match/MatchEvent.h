/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MatchEvent.h
 * @brief Header file for the MatchEvent class.
 * 
 * ARCHITECTURAL NOTES:
 * - MatchEvent is a class that represents a match event in Hydragon.
 * - It is used to represent any match event in the game world, such as match start, match end, etc.
 * - It supports interactive features, such as triggering events and responding to events. Also supports two-way messaging with other match events, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct MatchEventInfo : public NodeInfo {
    MatchEventInfo() {
        nodeType = "Gameplay/MatchEvent";
        
        inputs = {
            "eventType",       // Type of match event (match start, match end, etc.)
            "eventData",       // Match event data
            "environment",     // Environment data
            "characterData",   // Character data
            "eventState"       // Match event state
        };
        
        outputs = {
            "eventStatus",     // Match event status
            "eventMetrics"     // Match event performance metrics
        };
    }
};

class MatchEvent : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MatchEvent(const MatchEventInfo& info = MatchEventInfo())
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
    ~MatchEvent() = default;     // Default destructor
};

} // namespace hd

