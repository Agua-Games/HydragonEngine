/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InteractionEvent.h
 * @brief Header file for the InteractionEvent class.
 * 
 * ARCHITECTURAL NOTES:
 * - InteractionEvent is a class that represents an interaction event in Hydragon.
 * - It is useful for registering interaction events in the game world, checking for global flags before and after the interaction, setting global flags, querying and
 * setting states of nodes, etc. It also handles the interaction state, etc.
 * - For the sake of simplicity it handles not only the event but also other interaction data, conditionals, etc. It is like a hub to link a prop, device, vehicle, etc,
 * with the interaction manager, intermediated by the settings, specifics of the interactionMechanics.
 * - It is used to represent any interaction event in the game world, such as player interactions, character interactions, etc.
 * - It supports interactive features, such as triggering events and responding to events. Also supports two-way messaging with other interaction events, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Item.h"

namespace hd {

struct InteractionEventInfo : public NodeInfo {
    InteractionEventInfo() {
        nodeType = "Gameplay/InteractionEvent";
        
        inputs = {
            "eventType",       // Type of interaction event (player interaction, character interaction, etc.)
            "eventData",       // Interaction event data
            "environment",     // Environment data
            "characterData",   // Character data
            "eventState"       // Interaction event state
        };
        
        outputs = {
            "eventStatus",     // Interaction event status
            "eventMetrics"     // Interaction event performance metrics
        };
    }
};

class InteractionEvent : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit InteractionEvent(const InteractionEventInfo& info = InteractionEventInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    int difficulty = 0;
    std::unordered_map<std::string, int> globalFlags;
    DataTable eventData;
    bool requireItem;
    Item requiredItem;

    // === Processing ===
    void processNode() override {
 
    }
    void triggerEvent();
    void broadcastEvent();
    void messageEvent();
    void respondToEvent();
    void checkGlobalFlags();
    void setGlobalFlags();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~InteractionEvent() = default;     // Default destructor
};

} // namespace hd

