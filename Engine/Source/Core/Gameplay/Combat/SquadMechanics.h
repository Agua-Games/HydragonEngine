/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SquadMechanics.h
 * @brief Header file for the SquadMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - SquadMechanics is a class that represents squad mechanics in Hydragon.
 * - It is used to represent any squad mechanics in the game world, such as squad management, squad communication, squad objectives, etc.
 * - It supports interactive features, such as squad management, squad communication, squad objectives, etc. Also supports two-way messaging with other squads, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"
#include "AgentAI.h"

namespace hd {

struct SquadMechanicsInfo : public BehaviorTreeInfo {
    SquadMechanicsInfo() {
        nodeType = "Gameplay/SquadMechanics";
        
        inputs = {
            "squadData",        // Squad data
            "environment",      // Environment data
            "characterData",    // Character data
            "squadState"        // Squad state
        };
        
        outputs = {
            "squadStatus",      // Squad status
            "squadMetrics"      // Squad performance metrics
        };
    }
};

class SquadMechanics : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit SquadMechanics(const SquadMechanicsInfo& info = SquadMechanicsInfo())
        : BehaviorTree(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void processSquadMechanics();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SquadMechanics() = default;     // Default destructor
};

} // namespace hd
