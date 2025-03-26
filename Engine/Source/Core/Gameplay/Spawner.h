/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Spawner.h
 * @brief Header file for the Spawner class.
 * 
 * ARCHITECTURAL NOTES:
 * - Spawner is a class that represents a spawner in Hydragon.
 * - It is used to spawn, manage, modify, enhance the behavior of game objects.
 * - It has built-in support for procedural features, such as spawning, respawning, and object management. Also supports two-way messaging with other objects, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct SpawnerInfo : public NodeInfo {
    SpawnerInfo() {
        nodeType = "Gameplay/Spawner";
        
        inputs = {
            "spawnType",       // Type of object to spawn
            "spawnPosition",    // Position to spawn the object
            "spawnRotation",    // Rotation to spawn the object
            "spawnParams",      // Additional spawn parameters
            "behaviorTree",     // Behavior tree for the object
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "spawnedObject",    // Spawned object
            "spawnStatus",      // Spawn status
            "objectMetrics"     // Object performance metrics
        };
    }
};

class Spawner : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Spawner(const SpawnerInfo& info = SpawnerInfo())
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
    ~Spawner() = default;     // Default destructor
};

} // namespace hd