/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HorseAI.h
 * @brief Header file for the HorseAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - HorseAI is a class that represents a horse AI in Hydragon.
 * - It is used to represent and process horse AI.
 * - It uses the Vulkan API for horse AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct HorseAIInfo : public NodeInfo {
    HorseAIInfo() {
        nodeType = "Gameplay/HorseAI";
        
        inputs = {
            "horseData",        // Horse data
            "environment",      // Environment data
            "characterData",    // Character data
            "horseState"        // Horse state
        };
        
        outputs = {
            "horseStatus",      // Horse status
            "horseMetrics"      // Horse performance metrics
        };
    }
};

class HorseAI : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit HorseAI(const HorseAIInfo& info = HorseAIInfo())
        : BehaviorTree(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processHorseAI();
    void processNode() override {
        processHorseAI(); 
    }    
    void update() override {
        processHorseAI(); 
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~HorseAI() = default;     // Default destructor
};

} // namespace hd