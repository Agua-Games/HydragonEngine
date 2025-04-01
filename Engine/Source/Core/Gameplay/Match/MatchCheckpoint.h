/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MatchCheckpoint.h
 * @brief Header file for the MatchCheckpoint class.
 * 
 * ARCHITECTURAL NOTES:
 * - MatchCheckpoint is a class that represents a match checkpoint in Hydragon.
 * - It is used to represent any match checkpoint in the game world, such as match checkpoints, match save points, etc.
 * - It supports interactive features, such as saving, loading, and resetting. Also supports two-way messaging with other match checkpoints, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct MatchCheckpointInfo : public NodeInfo {
    MatchCheckpointInfo() {
        nodeType = "Gameplay/MatchCheckpoint";
        
        inputs = {
            "checkpointType",  // Type of match checkpoint (match checkpoint, match save point, etc.)
            "checkpointData",  // Match checkpoint data
            "environment",     // Environment data
            "characterData",   // Character data
            "checkpointState"  // Match checkpoint state
        };
        
        outputs = {
            "checkpointStatus",  // Match checkpoint status
            "checkpointMetrics"  // Match checkpoint performance metrics
        };
    }
};

class MatchCheckpoint : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MatchCheckpoint(const MatchCheckpointInfo& info = MatchCheckpointInfo())
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
    ~MatchCheckpoint() = default;     // Default destructor
};

} // namespace hd

