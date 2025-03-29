/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Training.h
 * @brief Header file for the Training class.
 * 
 * ARCHITECTURAL NOTES:
 * - Training is a class that represents training in Hydragon.
 * - It is used to represent any training in the game world, such as combat training, magic training, etc.
 * - It supports interactive features, such as training, learning, and upgrading. Also supports two-way messaging with other training, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct TrainingInfo : public NodeInfo {
    TrainingInfo() {
        nodeType = "Gameplay/Training";
        
        inputs = {
            "trainingType",    // Type of training (combat, magic, etc.)
            "trainingData",    // Training data
            "environment",     // Environment data
            "characterData",   // Character data
            "trainingState"    // Training state
        };
        
        outputs = {
            "trainingStatus",  // Training status
            "trainingMetrics"  // Training performance metrics
        };
    }
};

class Training : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Training(const TrainingInfo& info = TrainingInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addTraining(const std::string& trainingName);
    void removeTraining(const std::string& trainingName);
    void processTraining();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Training() = default;     // Default destructor
};

} // namespace hd