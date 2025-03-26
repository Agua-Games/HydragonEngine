/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file Checkpoint.h
 * @brief Header file for the Checkpoint class.
 * 
 * ARCHITECTURAL NOTES:
 * - Checkpoint is a class that represents a checkpoint in Hydragon.
 * - It is used to represent any checkpoint in the game world, such as save points, checkpoints, etc.
 * - It supports interactive features, such as saving, loading, and resetting. Also supports two-way messaging with other checkpoints, environment, character, etc.
 */
#pragma once

#include "Node.h"

namespace hd {

struct CheckpointInfo : public NodeInfo {
    CheckpointInfo() {
        nodeType = "Gameplay/Checkpoint";
        
        inputs = {
            "checkpointType",  // Type of checkpoint (save point, checkpoint, etc.)
            "checkpointData",  // Checkpoint data
            "environment",     // Environment data
            "characterData",   // Character data
            "checkpointState"  // Checkpoint state
        };
        
        outputs = {
            "checkpointStatus",  // Checkpoint status
            "checkpointMetrics"  // Checkpoint performance metrics
        };
    }
};

class Checkpoint : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Checkpoint(const CheckpointInfo& info = CheckpointInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
        checkpointType = getInputValue<CheckpointType>("checkpointType");
        checkpointData = getInputValue<CheckpointData>("checkpointData");
        environment = getInputValue<Environment>("environment");
        characterData = getInputValue<CharacterData>("characterData");
        checkpointState = getInputValue<CheckpointState>("checkpointState");

        // Process checkpoint
        auto checkpointStatus = updateCheckpoint(checkpointType, checkpointData, environment, characterData, checkpointState);

        // Set outputs
        setOutputValue("checkpointStatus", checkpointStatus);
        setOutputValue("checkpointMetrics", computeCheckpointMetrics(checkpointStatus));
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Checkpoint() = default;     // Default destructor
};

} // namespace hd

