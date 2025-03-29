/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DialogueManager.h
 * @brief Header file for the DialogueManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - DialogueManager is a singleton class that manages the dialogue in the game.
 * - It is responsible for updating the dialogue and managing the dialogue system.
 * - It uses the Vulkan API for dialogue management.
 */
#pragma once

#include "Node.h"

namespace hd {

struct DialogueManagerInfo : public NodeInfo {
    DialogueManagerInfo() {
        nodeType = "Gameplay/DialogueManager";
        
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

class DialogueManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DialogueManager(const DialogueManagerInfo& info = DialogueManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DialogueData dialogueData;
    DialogueState dialogueState;

    // === Processing ===
    void processNode() override {
        dialogueData = getInputValue<DialogueData>("dialogueData");
        dialogueState = getInputValue<DialogueState>("dialogueState");

        // Process dialogue
        auto dialogueStatus = updateDialogue(dialogueData, dialogueState);

        // Set outputs
        setOutputValue("dialogueStatus", dialogueStatus);
        setOutputValue("dialogueMetrics", computeDialogueMetrics(dialogueStatus));
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DialogueManager() = default;     // Default destructor
};

} // namespace hd

