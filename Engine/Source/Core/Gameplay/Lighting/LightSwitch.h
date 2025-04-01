/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LightSwitch.h
 * @brief Header file for the LightSwitch class.
 * 
 * ARCHITECTURAL NOTES:
 * - LightSwitch is a class that represents a light switch in Hydragon.
 * - It is used to represent any light switch in the game world, such as light switches, etc.
 * - It supports interactive features, such as turning lights on and off. Also supports two-way messaging with other light switches, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct LightSwitchInfo : public NodeInfo {
    LightSwitchInfo() {
        nodeType = "Gameplay/LightSwitch";
        
        inputs = {
            "lightSwitchType",  // Type of light switch (light switch, etc.)
            "lightSwitchData",  // Light switch data
            "environment",      // Environment data
            "characterData",    // Character data
            "lightSwitchState"  // Light switch state
        };
        
        outputs = {
            "lightSwitchStatus",  // Light switch status
            "lightSwitchMetrics"  // Light switch performance metrics
        };
    }
};

class LightSwitch : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit LightSwitch(const LightSwitchInfo& info = LightSwitchInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LightSwitch() = default;     // Default destructor
};

} // namespace hd
