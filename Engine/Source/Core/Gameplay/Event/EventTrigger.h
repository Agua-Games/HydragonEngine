/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EventTrigger.h
 * @brief Header file for the EventTrigger class.
 * 
 * ARCHITECTURAL NOTES:
 * - EventTrigger is a class that represents an event trigger in Hydragon.
 * - It is used to represent any event trigger in the game world, such as buttons, switches, levers, etc.
 * - It supports interactive features, such as triggering events and responding to events. Also supports two-way messaging with other event triggers, environment, character, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct EventTriggerInfo : public NodeInfo {
    EventTriggerInfo() {
        nodeType = "Gameplay/EventTrigger";
        
        inputs = {
            "triggerType",     // Type of event trigger (button, switch, lever, etc.)
            "triggerData",     // Event trigger data
            "environment",     // Environment data
            "characterData",   // Character data
            "triggerState"     // Event trigger state
        };
        
        outputs = {
            "triggerStatus",   // Event trigger status
            "triggerMetrics"   // Event trigger performance metrics
        };
    }
};

class EventTrigger : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit EventTrigger(const EventTriggerInfo& info = EventTriggerInfo())
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
    ~EventTrigger() = default;     // Default destructor
};

} // namespace hd

