/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScoreMechanics.h
 * @brief Header file for the ScoreMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - ScoreMechanics is a class that represents casual mechanics in Hydragon.
 * - It is used to represent any casual mechanics in the game world, such as points, scores, etc.
 * - It supports interactive features, such as scoring, ranking, and leaderboards. Also supports two-way messaging with other casual mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct ScoreMechanicsInfo : public NodeInfo {
    ScoreMechanicsInfo() {
        nodeType = "Gameplay/ScoreMechanics";
        
        inputs = {
            "casualData",        // Casual data
            "environment",       // Environment data
            "characterData",     // Character data
            "casualState"        // Casual state
        };
        
        outputs = {
            "casualStatus",      // Casual status
            "casualMetrics"      // Casual performance metrics
        };
    }
};

class ScoreMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit ScoreMechanics(const ScoreMechanicsInfo& info = ScoreMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ScoreMechanics() = default;     // Default destructor
};

} // namespace hd
