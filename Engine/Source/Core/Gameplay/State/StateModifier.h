/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StateModifier.h
 * @brief Header file for the StateModifier class.
 * 
 * ARCHITECTURAL NOTES:
 * - StateModifier is a class that represents a state modifier in Hydragon.
 * - It is used to modify the state of characters, vehicles, etc, in the game world.
 * - It supports various types of state modifications, such as health, mana, movement, etc.
 * - It supports interactive features, such as modifying, updating, and accessing. Also supports two-way messaging with other state modifiers, environment, character, etc.
 */

#pragma once

#include "Node.h"

namespace hd {

struct StateModifierInfo : public NodeInfo {
    StateModifierInfo() {
        NodeType = "StateModifier";
        
        inputs = {
            "StateType",       // Type of state (health, mana, movement, etc.)
            "StateData",       // State data
            "Environment",     // Environment data
            "CharacterData",   // Character data
            "StateModifier"    // State modifier data
        };
        
        outputs = {
            "StateStatus",     // State status
            "StateMetrics"     // State performance metrics
        };
    }
};

class StateModifier : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit StateModifier(const StateModifierInfo& info = StateModifierInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float value = 0.0f;
    float threshold = 0.0f;
    float duration = 0.0f;
    float tickRate = 0.0f;
    float movementPenalty = 0.0f;
    float damageReduction = 0.0f;          // Each subsequent target takes 20% less damage
    float defenseReduction = 0.0f;         // Each subsequent target takes 20% less defense
    float damageBonus = 0.0f;              // Each subsequent target takes 20% less damage

    // === Processing ===
    void processNode() override { }
    void modifyState();
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~StateModifier() = default;     // Default destructor
};

} // namespace hd
