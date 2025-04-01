/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RepairMechanics.h
 * @brief Header file for the RepairMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - RepairMechanics is a class that represents repair mechanics in Hydragon.
 * - It is used to represent any repair mechanics in the game world, such as repairing, maintaining, etc.
 * - It supports interactive features, such as repairing, maintaining, and upgrading. Also supports two-way messaging with other repair mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RepairMechanicsInfo : public NodeInfo {
    RepairMechanicsInfo() {
        nodeType = "Gameplay/RepairMechanics";
        
        inputs = {
            "repairData",        // Repair data
            "environment",       // Environment data
            "characterData",     // Character data
            "repairState"        // Repair state
        };
        
        outputs = {
            "repairStatus",      // Repair status
            "repairMetrics"      // Repair performance metrics
        };
    }
};

class RepairMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RepairMechanics(const RepairMechanicsInfo& info = RepairMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void repair();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RepairMechanics() = default;     // Default destructor
};

} // namespace hd

