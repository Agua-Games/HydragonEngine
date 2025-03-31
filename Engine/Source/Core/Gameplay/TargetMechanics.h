/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TargetMechanics.h
 * @brief Header file for the TargetMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - TargetMechanics is a class that represents target mechanics in Hydragon.
 * - It is used to represent any target mechanics in the game world, such as targeting, tracking, etc.
 * - It supports interactive features, such as targeting, tracking, and locking on. Also supports two-way messaging with other target mechanics, environment, character, etc.
 */

#pragma once
#include "Engine.h"
#include "Node.h"
#include "Target.h"

namespace hd {

struct TargetMechanicsInfo : public NodeInfo {
    TargetMechanicsInfo() {
        nodeType = "Gameplay/TargetMechanics";
        
        inputs = {
            "targetMechanicsData",// Target mechanics data
            "environment",        // Environment data
            "characterData",      // Character data
            "targetMechanicsState"// Target mechanics state
        };
        
        outputs = {
            "targetMechanicsStatus",// Target mechanics status
            "targetMechanicsMetrics"// Target mechanics performance metrics
        };
    }
};

class TargetMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit TargetMechanics(const TargetMechanicsInfo& info = TargetMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable targetMechanicsData;
    std::vector<Target> targets;
    float range = 0.0f;
    float accuracy = 0.0f;
    float lockTime = 0.0f;
    bool thermalVision = false;
    DataTable targetMechanicsState;
    
    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TargetMechanics() = default;     // Default destructor
};

} // namespace hd