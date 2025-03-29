/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DialogueCheckpoint.h
 * @brief Header file for the DialogueCheckpoint class.
 * 
 * ARCHITECTURAL NOTES:
 * - DialogueCheckpoint is a class that represents a dialogue checkpoint in Hydragon.
 * - It is used to represent any dialogue checkpoint in the game world, such as dialogue checkpoints, dialogue save points, etc.
 * - It supports interactive features, such as saving, loading, and resetting. Also supports two-way messaging with other dialogue checkpoints, environment, character, etc.
 */

#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct DialogueCheckpointInfo : public NodeInfo {
    DialogueCheckpointInfo() {
        nodeType = "Gameplay/DialogueCheckpoint";
        
        inputs = {
            "dialogueData",  // Dialogue data
            "dialogueState"  // Dialogue state
        };
        
        outputs = {
            "dialogueData",  // Dialogue data
            "dialogueStatus",  // Dialogue status
            "dialogueMetrics"  // Dialogue performance metrics
        };
    }
};

class DialogueCheckpoint : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DialogueCheckpoint(const DialogueCheckpointInfo& info = DialogueCheckpointInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void saveDialogue();
    void loadDialogue();
    void resetDialogue();
    void processDialogue();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DialogueCheckpoint() = default;     // Default destructor
};

} // namespace hd
