/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Experience.h
 * @brief Header file for the Experience class.
 * 
 * ARCHITECTURAL NOTES:
 * - Experience is a class that represents experience in Hydragon.
 * - It is used to represent any experience in the game world, such as player experience, enemy experience, etc.
 * - It supports interactive features, such as leveling, upgrading, and unlocking. Also supports two-way messaging with other experience, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "MemoryBank.h"

namespace hd {

struct ExperienceInfo : public NodeInfo {
    ExperienceInfo() {
        nodeType = "Gameplay/Experience";
        
        inputs = {
            "xpData",        // Experience data
            "characterData", // Character data
            "xpState"        // Experience state
        };
        
        outputs = {
            "xpStatus",      // Experience status
            "xpMetrics"      // Experience performance metrics
        };
    }
};

class Experience : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Experience(const ExperienceInfo& info = ExperienceInfo())
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
    ~Experience() = default;     // Default destructor
};

} // namespace hd
