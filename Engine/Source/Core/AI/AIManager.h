/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AIManager.h
 * @brief Header file for the AIManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - AIManager is a class that represents an AI manager in Hydragon.
 * - It is used to manage AI agents, behavior trees, and other AI-related systems.
 * - It supports interactive features, such as AI agent management, behavior tree management, and AI system management. Also supports two-way messaging with other AI systems, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "DataTable.h"
#include "BehaviorTree.h"

namespace hd {

struct AIManagerInfo : public NodeInfo {
    AIManagerInfo() {
        nodeType = "AI/AIManager";
        
        inputs = {
            "aiData",        // AI data
            "environment",   // Environment data
            "characterData", // Character data
            "aiState"        // AI state
        };
        
        outputs = {
            "aiStatus",      // AI status
            "aiMetrics"      // AI performance metrics
        };
    }
};

class AIManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AIManager(const AIManagerInfo& info = AIManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable aiData;
    DataTable environment;
    DataTable characterData;
    DataTable aiState;

    // === Processing ===
    void processNode() override {
        aiData = getInputValue<DataTable>("aiData");
        environment = getInputValue<DataTable>("environment");
        characterData = getInputValue<DataTable>("characterData");
        aiState = getInputValue<DataTable>("aiState");

        // Process AI manager
        auto aiStatus = updateAIManager(aiData, environment, characterData, aiState);

        // Set outputs
        setOutputValue("aiStatus", aiStatus);
        setOutputValue("aiMetrics", computeAIMetrics(aiStatus));
    } 
    void processAgentAI();
    void updateAIManager(const DataTable& aiData, const DataTable& environment, const DataTable& characterData, const DataTable& aiState);
    void computeAIMetrics(const DataTable& aiStatus);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AIManager() = default;     // Default destructor
};

} // namespace hd