/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DialogueBranch.h
 * @brief Header file for the DialogueBranch class.
 * 
 * ARCHITECTURAL NOTES:
 * - DialogueBranch is a class that represents a dialogue branch in Hydragon.
 * - Dialogue branch is a versatile self contained dialogue "atom" (a statement by a character). At the same time it can be used as a single statement by a character,
 * or a monologue, it can be used as a fork leading to dialogue choices (its outputs), and thus chained together to form a dialogue tree.
 * - It works together with the DialogueCheckpoint to form a complete modular versatile dialogue system with the ability to store checkpoint flags for gameplay events
 * (e.g. "information sent to player") and send information exchanged in the dialogue to external nodes, like the MemoryBank.
 * - It is used to represent any dialogue branch in the game world, such as player dialogue branches, enemy dialogue branches, etc.
 * - It supports interactive features, such as branching, decision making, and dialogue flow. Also supports two-way messaging with other dialogue branches, environment, character, etc.
 */

#pragma once

#include "Node.h"
#include "DataTable.h"

namespace hd {

struct DialogueBranchInfo : public NodeInfo {
    DialogueBranchInfo() {
        nodeType = "Gameplay/DialogueBranch";
        
        inputs = {
            "dialogueData",  // Dialogue data
            "dialogueState"  // Dialogue state
        };
        
        outputs = {
            "dialogueStatus",  // Dialogue status
            "dialogueMetrics"  // Dialogue performance metrics
        };
    }
};

class DialogueBranch : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DialogueBranch(const DialogueBranchInfo& info = DialogueBranchInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void processDialogue();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DialogueBranch() = default;     // Default destructor
};

} // namespace hd
