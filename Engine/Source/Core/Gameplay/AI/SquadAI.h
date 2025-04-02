/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SquadAI.h
 * @brief Header file for the SquadAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - SquadAI is a class that represents a squad AI in Hydragon.
 * - It is used to represent and process squad AI.
 * - It uses the Vulkan API for squad AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 * @todo Decide if deriving from BehaviorTree or AgentAI.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "TacticalAI.h"
#include "AgentAI.h"

namespace hd {

struct SquadAIInfo : public TacticalAIInfo {
    SquadAIInfo() {
        nodeType = "Gameplay/SquadAI";
        
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

class SquadAI : public TacticalAI {
public:
    // === Allocation, Initialization, Loading ===
    explicit SquadAI(const SquadAIInfo& info = SquadAIInfo())
        : SquadAI(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void processSquadAI();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SquadAI() = default;     // Default destructor
};

} // namespace hd
