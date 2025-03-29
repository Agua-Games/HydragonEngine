/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TrainingMechanics.h
 * @brief Header file for the TrainingMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - TrainingMechanics is a class that represents training mechanics in Hydragon.
 * - It is used to represent any training mechanics in the game world, such as training, learning, etc.
 * - It supports interactive features, such as training, learning, and upgrading. Also supports two-way messaging with other training mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct TrainingMechanicsInfo : public NodeInfo {
    TrainingMechanicsInfo() {
        nodeType = "Gameplay/TrainingMechanics";
        
        inputs = {
            "trainingData",        // Training data
            "environment",         // Environment data
            "characterData",       // Character data
            "trainingState"        // Training state
        };
        
        outputs = {
            "trainingStatus",      // Training status
            "trainingMetrics"      // Training performance metrics
        };
    }
};

class TrainingMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit TrainingMechanics(const TrainingMechanicsInfo& info = TrainingMechanicsInfo())
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
    ~TrainingMechanics() = default;     // Default destructor
};

} // namespace hd