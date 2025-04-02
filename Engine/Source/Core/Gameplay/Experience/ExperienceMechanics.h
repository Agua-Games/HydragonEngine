/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ExperienceMechanics.h
 * @brief Header file for the ExperienceMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - ExperienceMechanics is a class that represents experience mechanics in Hydragon.
 * - It is used to represent any experience mechanics in the game world, such as player experience, enemy experience, etc.
 * - It supports interactive features, such as leveling, upgrading, and unlocking. Also supports two-way messaging with other experience mechanics, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "MemoryBank.h"

namespace hd {

struct ExperienceMechanicsInfo : public NodeInfo {
    ExperienceMechanicsInfo() {
        nodeType = "Gameplay/ExperienceMechanics";
        
        inputs = {
            "xpMechanicsData",// Experience mechanics data
            "environment",    // Environment data
            "characterData",  // Character data
            "xpMechanicsState"// Experience mechanics state
        };
        
        outputs = {
            "xpMechanicsStatus",// Experience mechanics status
            "xpMechanicsMetrics"// Experience mechanics performance metrics
        };
    }
};

class ExperienceMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ExperienceMechanics(const ExperienceMechanicsInfo& info = ExperienceMechanicsInfo())
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
    ~ExperienceMechanics() = default;     // Default destructor
};

} // namespace hd
