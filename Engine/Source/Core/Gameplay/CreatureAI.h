/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CreatureAI.h
 * @brief Header file for the CreatureAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - CreatureAI is a class that represents a creature AI in Hydragon.
 * - It is used to represent and process creature AI.
 * - It uses the Vulkan API for creature AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct CreatureAIInfo : public CharacterInfo {
    CreatureAIInfo() {
        nodeType = "Gameplay/CreatureAI";
        
        inputs = {
            "creature",         // Creature data
            "behaviorTree",     // Behavior tree data
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "creatureState",    // Creature state
            "behaviorMetrics",  // Behavior tree performance metrics
            "proceduralData"    // Generated procedural data
        };
    }
};

class CreatureAI : public Character {
public:
    // === Allocation, Initialization, Loading ===
    explicit CreatureAI(const CreatureAIInfo& info = CreatureAIInfo())
        : Character(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float aggression = 0.5f;
    float intelligence = 0.5f;
    float speed = 0.5f;
    float health = 100.0f;
    float mana = 100.0f;
    float stamina = 100.0f;
    float territoryRadius = 10.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CreatureAI() = default;     // Default destructor
};

} // namespace hd
