/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Character.h
 * @brief Header file for the Character class.
 * 
 * ARCHITECTURAL NOTES:
 * - Character is a class that represents a character controller in Hydragon.
 * - It is used to control, manage, modify, enhance the behavior of characters.
 * - It has built-in support for procedural features, such as pathfinding, decision making, etc. Also procedural emergent behaviors, self-modification, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct CharacterInfo : public BehaviorTreeInfo {
    CharacterInfo() {
        nodeType = "Gameplay/Character";
        
        inputs = {
            "character",        // Character data
            "behaviorTree",     // Behavior tree data
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "characterState",   // Character state
            "behaviorMetrics",  // Behavior tree performance metrics
            "proceduralData"    // Generated procedural data
        };
    }
};

/**
 * @class Character
 * @brief Represents a character controller node in the engine's node graph.
 */
class Character : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit Character(const CharacterInfo& info = CharacterInfo())
        : BehaviorTree(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // Set default values
    Character character;
    BehaviorTree behaviorTree;
    Environment environment;
    ProceduralParams proceduralParams;
    CharacterState characterState;
    BehaviorMetrics behaviorMetrics;
    ProceduralData proceduralData;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
        character = getInputValue<Character>("character");
        behaviorTree = getInputValue<BehaviorTree>("behaviorTree");
        environment = getInputValue<Environment>("environment");
        proceduralParams = getInputValue<ProceduralParams>("proceduralParams");

        // Process character controller
        auto characterState = updateCharacterController(character, behaviorTree, environment, proceduralParams);

        // Set outputs
        setOutputValue("characterState", characterState);
        setOutputValue("behaviorMetrics", computeBehaviorMetrics(characterState));
        setOutputValue("proceduralData", generateProceduralData(characterState));

    }
    void processCharacterController();
    void () override {
        processCharacterController(); 
    }
    void update() override { // Override the update() function to call processCharacterController() instead of update().
        processCharacterController(); // Call the processCharacterController() function to update the character controller state.
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Character() = default;     // Default destructor
};

} // namespace hd


