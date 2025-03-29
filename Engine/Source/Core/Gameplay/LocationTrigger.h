/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LocationTrigger.h
 * @brief Header file for the LocationTrigger class.
 * 
 * ARCHITECTURAL NOTES:
 * - LocationTrigger is a class that represents a location trigger in Hydragon.
 * - It is used to represent any location trigger in the game world, such as checkpoints, objectives, etc.
 * - It supports interactive features, such as triggering events and responding to events. Also supports two-way messaging with other location triggers, environment, character, etc.
 */
#pragma once
#include <vector>
#include "Node.h"
#include "DataTable.h"
#include "Trigger.h"

namespace hd {

struct Location {
    std::string name;
    std::string description;
    std::vector<std::string> objectives;
    std::vector<std::string> rewards;
}

enum class LocationTriggerType {
    checkpoint,
    objective,
    quest,
    event,
    custom
};

struct LocationTriggerInfo : public TriggerInfo {
    LocationTriggerInfo() {
        nodeType = "Gameplay/LocationTrigger";
        
        inputs = {
            "triggerType",     // Type of location trigger (checkpoint, objective, etc.)
            "triggerData",     // Location trigger data
            "environment",     // Environment data
            "characterData",   // Character data
            "triggerState"     // Location trigger state
        };
        
        outputs = {
            "triggerStatus",   // Location trigger status
            "triggerMetrics"   // Location trigger performance metrics
        };
    }
};

class LocationTrigger : public Trigger {
public:
    // === Allocation, Initialization, Loading ===
    explicit LocationTrigger(const LocationTriggerInfo& info = LocationTriggerInfo())
        : Trigger(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LocationTrigger() = default;     // Default destructor
};

} // namespace hd


