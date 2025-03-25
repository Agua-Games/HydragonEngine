/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PlayerController.h
 * @brief Header file for the PlayerController class.
 * 
 * ARCHITECTURAL NOTES:
 * - PlayerController is a class that represents a player controller in Hydragon.
 * - It is used to control, manage, modify, enhance the behavior of player characters.
 * - It has built-in support for blend of player explicit input with procedural features, such as pathfinding, decision making, etc, to achieve various degrees
 * of autonomy, player agency, etc. Also procedural emergent behaviors, self-modification, among other things.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct PlayerControllerInfo : public BehaviorTreeInfo {
    PlayerControllerInfo() {
        NodeType = "Gameplay/PlayerController";
        
        inputs = {
            "Player",           // Player data
            "BehaviorTree",     // Behavior tree data
            "Environment",      // Environment data
            "ProceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "PlayerState",      // Player state
            "BehaviorMetrics",  // Behavior tree performance metrics
            "ProceduralData"    // Generated procedural data
        };
    }
};

class PlayerController : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit PlayerController(const PlayerControllerInfo& info = PlayerControllerInfo())
        : BehaviorTree(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // === Processing ===
    void processPlayerController();
    void processNodeGraph() override {
        processPlayerController(); 
    }    
    void update() override { // Override the update() function to call processPlayerController() instead of update().
        processPlayerController(); // Call the processPlayerController() function to update the player controller state.
    }
    
    // === Cleanup === 
    void unload() override {}
    void cleanup() override {}
    ~PlayerController() = default;     // Default destructor
};

} // namespace hd
