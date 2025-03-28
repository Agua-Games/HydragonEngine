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
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Scene.h"
#include "BehaviorTree.h"
#include "AnimationController.h"

namespace hd {

struct CharacterInfo : public SceneInfo {
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
class Character : public Scene {
public:
    // === Allocation, Initialization, Loading ===
    explicit Character(const CharacterInfo& info = CharacterInfo())
        : Scene(info), behaviorTree(info) {} 
    initialize() override {}
    load() override {}

    // Set default values
    Character character;
    BehaviorTree behaviorTree;
    AnimationController animationController;
    Environment environment;
    ProceduralParams proceduralParams;
    CharacterState characterState;
    BehaviorMetrics behaviorMetrics;
    ProceduralData proceduralData;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
        // Get inputs
        auto environment = getInputValue<Environment>("environment");
        auto params = getInputValue<ProceduralParams>("proceduralParams");

        // Update AI
        behaviorTree.process(environment, params);
        
        // Update character state
        characterState = updateCharacterState();
        
        // Set outputs
        setOutputValue("characterState", characterState);
        setOutputValue("behaviorMetrics", behaviorTree.getMetrics());
        setOutputValue("proceduralData", generateProceduralData());
    }
    // Character gets Scene functionality through BehaviorTree
    // Can manage equipment, attachments, etc. as sub-scenes
    void equipItem(const Scene* item) {
        attachScene(item, "equipment_slot");
    }

    // Sketch example of exposing only relevant AI controls
    void setAggression(float value) { 
        behaviorTree.setParameter("aggression", value); 
    }

    void processAnimationController();
    void update() override {
        processAnimationController();
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Character() = default;     // Default destructor
};

} // namespace hd


