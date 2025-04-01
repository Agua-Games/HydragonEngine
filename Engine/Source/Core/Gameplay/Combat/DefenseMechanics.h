/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DefenseMechanics.h
 * @brief Header file for the DefenseMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - DefenseMechanics is a class that represents defense mechanics in Hydragon.
 * - It is used to represent any defense mechanics in the game world, such as turrets, walls, etc.
 * - It supports interactive features, such as defending, attacking, and patrolling. Also supports two-way messaging with other defense mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct DefenseMechanicsInfo : public NodeInfo {
    DefenseMechanicsInfo() {
        nodeType = "Gameplay/DefenseMechanics";
        
        inputs = {
            "defenseData",        // Defense data
            "environment",        // Environment data
            "characterData",      // Character data
            "defenseState"        // Defense state
        };
        
        outputs = {
            "defenseStatus",      // Defense status
            "defenseMetrics"      // Defense performance metrics
        };
    }
};

class DefenseMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit DefenseMechanics(const DefenseMechanicsInfo& info = DefenseMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addTurret(const std::string& turretName);
    void addElement(const std::string& elementName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DefenseMechanics() = default;     // Default destructor
};

} // namespace hd
