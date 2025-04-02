/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TacticalAI.h
 * @brief Header file for the TacticalAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - TacticalAI is a class that represents a tactical AI in Hydragon.
 * - It is used to represent and process tactical AI.
 * - It uses the Vulkan API for tactical AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct TacticalAIInfo : public BehaviorTreeInfo {
    TacticalAIInfo() {
        nodeType = "Gameplay/TacticalAI";
        
        inputs = {
            "tacticalData",    // Tactical data
            "environment",     // Environment data
            "characterData",   // Character data
            "tacticalState"    // Tactical state
        };
        
        outputs = {
            "tacticalStatus",  // Tactical status
            "tacticalMetrics"  // Tactical performance metrics
        };
    }
};

class TacticalAI : public BehaviorTree {
public:
    // === Struct Definitions ===
    enum class TacticalBehavior {
        Aggressive,
        Defensive,
        Cautious,
        Other
    };

    // === Allocation, Initialization, Loading ===
    explicit TacticalAI(const TacticalAIInfo& info = TacticalAIInfo())
        : BehaviorTree(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    TacticalBehavior behavior = TacticalBehavior::Aggressive;
    bool suppressiveFire = false;
    bool flanking = false;
    bool ambush = false;
    bool infiltration = false;
    bool reconnaissance = false;
    bool patrolling = false;

    // === Processing ===
    void processTacticalAI();
    void processNode() override {
        processTacticalAI(); 
    }    
    void update() override {
        processTacticalAI(); 
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TacticalAI() = default;     // Default destructor
};

} // namespace hd

