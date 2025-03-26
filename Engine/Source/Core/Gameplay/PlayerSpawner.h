/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PlayerSpawner.h
 * @brief Header file for the PlayerSpawner class.
 * 
 * ARCHITECTURAL NOTES:
 * - PlayerSpawner is a class that represents a player spawner in Hydragon.
 * - It is used to spawn, manage, modify, enhance the behavior of player characters.
 * - It has built-in support for procedural features, such as spawning, respawning, and player management. Also supports two-way messaging with other players, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct PlayerSpawnerInfo : public SpawnerInfo {
    PlayerSpawnerInfo() {
        nodeType = "Gameplay/PlayerSpawner";
        
        inputs = {
            "playerType",       // Type of player to spawn
            "spawnPosition",    // Position to spawn the player
            "spawnRotation",    // Rotation to spawn the player
            "spawnParams",      // Additional spawn parameters
            "behaviorTree",     // Behavior tree for the player
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "spawnedPlayer",    // Spawned player object
            "spawnStatus",      // Spawn status
            "playerMetrics"     // Player performance metrics
        };
    }
};

class PlayerSpawner : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit PlayerSpawner(const PlayerSpawnerInfo& info = PlayerSpawnerInfo())
        : BehaviorTree(info) {}    // Call the base class constructor
    initialize() override {}       // Override the initialize() function
    load() override {}             // Override the load() function

    // === Processing ===
    void processNode() override {
        playerType = getInputValue<PlayerType>("playerType");
        spawnPosition = getInputValue<vec3>("spawnPosition");
        spawnRotation = getInputValue<quat>("spawnRotation");
        spawnParams = getInputValue<SpawnParams>("spawnParams");
        behaviorTree = getInputValue<BehaviorTree>("behaviorTree");
        environment = getInputValue<Environment>("environment");
        proceduralParams = getInputValue<ProceduralParams>("proceduralParams");

        // Spawn the player
        auto spawnedPlayer = spawnPlayer(playerType, spawnPosition, spawnRotation, spawnParams, behaviorTree, environment, proceduralParams);

        // Set outputs
        setOutputValue("spawnedPlayer", spawnedPlayer);
        setOutputValue("spawnStatus", computeSpawnStatus(spawnedPlayer));
        setOutputValue("playerMetrics", computePlayerMetrics(spawnedPlayer));
    }
    void spawnPlayer();
    void () override {
        spawnPlayer(); 
    }
    void update() override {
        // Update the player spawner
        updatePlayerSpawner();
    }
    void updatePlayerSpawner();

    // === Cleanup ===
    void unload() override {}       // Override the unload() function
    void cleanup() override {}     // Override the cleanup() function
    ~PlayerSpawner() = default;    // Default destructor
};

} // namespace hd