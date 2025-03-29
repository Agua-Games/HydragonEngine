/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InteractionManager.h
 * @brief Header file for the InteractionManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - InteractionManager is a singleton class that manages the interactions in the game.
 * - It is responsible for managing the interactions in the game.
 * - It supports interactive features, such as interacting with objects, characters, and environment. Also supports two-way messaging with other interactions, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "ProceduralManager.h"
#include "DataTable.h"
#include "Prop.h"
#include "Inventory.h"
#include "Character.h"
#include "Creature.h"
#include "Interaction.h"

namespace hd {

struct InteractionManagerInfo : public NodeInfo {
    InteractionManagerInfo() {
        nodeType = "Gameplay/InteractionManager";
        
        inputs = {
            "interactions",      // Array of interactions
            "interactionState",  // Current state of the interaction
            "interactionSpeed",  // Speed of the interaction
            "interactionLoop"    // Whether the interaction should loop
        };
        
        outputs = {
            "activeInteractions", // Active interactions
            "interactionMetrics"  // Performance and quality metrics
        };
    }
};

class InteractionManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit InteractionManager(const InteractionManagerInfo& info = InteractionManagerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addInteraction(Interaction* interaction);
    void removeInteraction(Interaction* interaction);
    void queryInteraction(Interaction* interaction);
    void processInteraction();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~InteractionManager() = default;     // Default destructor
};

} // namespace hd