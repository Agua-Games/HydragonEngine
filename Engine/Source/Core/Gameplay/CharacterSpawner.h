/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CharacterSpawner.h
 * @brief Header file for the CharacterSpawner class.
 * 
 * ARCHITECTURAL NOTES:
 * - CharacterSpawner is a class that represents a character spawner in Hydragon.
 * - It is used to spawn, manage, modify, enhance the behavior of characters.
 * - It has built-in support for procedural features, such as spawning, respawning, and character management. Also supports two-way messaging with other characters, environment, player, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct CharacterSpawnerInfo : public SpawnerInfo {
    CharacterSpawnerInfo() {
        nodeType = "Gameplay/CharacterSpawner";
        
        inputs = {
            "characterType",    // Type of character to spawn
            "spawnPosition",    // Position to spawn the character
            "spawnRotation",    // Rotation to spawn the character
            "spawnParams",      // Additional spawn parameters
            "behaviorTree",     // Behavior tree for the character
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "spawnedCharacter", // Spawned character object
            "spawnStatus",      // Spawn status
            "characterMetrics"  // Character performance metrics
        };
    }
};

class CharacterSpawner : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CharacterSpawner(const CharacterSpawnerInfo& info = CharacterSpawnerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CharacterSpawner() = default;     // Default destructor
};

} // namespace hd