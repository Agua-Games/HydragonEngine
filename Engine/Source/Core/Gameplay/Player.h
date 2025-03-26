/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Player.h
 * @brief Header file for the Player class.
 * 
 * ARCHITECTURAL NOTES:
 * - Player is a class that represents a player in Hydragon.
 * - It is used to control, manage, modify, enhance the behavior of player characters.
 * - It has built-in support for blend of player explicit input with procedural features, such as pathfinding, decision making, etc, to achieve various degrees
 * of autonomy, player agency, etc. Also procedural emergent behaviors, self-modification, among other things.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct PlayerInfo : public NodeInfo {
    PlayerInfo() {
        nodeType = "Gameplay/Player";
        
        inputs = {
            "player",           // Player data
            "behaviorTree",     // Behavior tree data
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "playerState",      // Player state
            "behaviorMetrics",  // Behavior tree performance metrics
            "proceduralData"    // Generated procedural data
        };
    }
};

/**
 * @class Player
 * @brief Represents a player controller node in the engine's node graph.
 */
class Player : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Player(const PlayerInfo& info = PlayerInfo())
        : BehaviorTree(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // Set default values
    Player player;
    BehaviorTree behaviorTree;
    Environment environment;
    ProceduralParams proceduralParams;
    PlayerState playerState;
    BehaviorMetrics behaviorMetrics;
    ProceduralData proceduralData;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
        player = getInputValue<Player>("player");
        behaviorTree = getInputValue<BehaviorTree>("behaviorTree");
        environment = getInputValue<Environment>("environment");
        proceduralParams = getInputValue<ProceduralParams>("proceduralParams");

        // Process player controller
        auto playerState = updatePlayerController(player, behaviorTree, environment, proceduralParams);

        // Set outputs
        setOutputValue("playerState", playerState);
        setOutputValue("behaviorMetrics", computeBehaviorMetrics(playerState));
        setOutputValue("proceduralData", generateProceduralData(playerState));

    }
    void processPlayerController();
    void () override {
        processPlayerController(); 
    }    
    void update() override { // Override the update() function to call processPlayerController() instead of update().
        processPlayerController(); // Call the processPlayerController() function to update the player controller state.
    }
    
    // === Cleanup === 
    void unload() override {}
    void cleanup() override {}
    ~Player() = default;     // Default destructor
};

} // namespace hd
