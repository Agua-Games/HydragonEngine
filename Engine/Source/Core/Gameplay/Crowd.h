/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Crowd.h
 * @brief Header file for the Crowd class.
 * 
 * ARCHITECTURAL NOTES:
 * - Crowd is a class that represents a crowd in Hydragon.
 * - It is used to represent any crowd in the game world, such as mobs, crowds, etc.
 * - It supports interactive features, such as flocking, schooling, and swarming. Also supports two-way messaging with other crowds, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CrowdInfo : public NodeInfo {
    CrowdInfo() {
        nodeType = "Gameplay/Crowd";
        
        inputs = {
            "crowdType",       // Type of crowd (mob, crowd, etc.)
            "crowdData",       // Crowd data
            "environment",     // Environment data
            "characterData",   // Character data
            "crowdState"       // Crowd state
        };
        
        outputs = {
            "crowdStatus",     // Crowd status
            "crowdMetrics"     // Crowd performance metrics
        };
    }
};

class Crowd : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Crowd(const CrowdInfo& info = CrowdInfo())
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
    ~Crowd() = default;     // Default destructor
};

} // namespace hd