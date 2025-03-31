/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CompanionMechanics.h
 * @brief Header file for the CompanionMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - CompanionMechanics is a class that represents companion mechanics in Hydragon.
 * - It is used to represent any companion mechanics in the game world, such as loyalty, bonding, etc.
 * - It supports interactive features, such as loyalty, bonding, and training. Also supports two-way messaging with other companion mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CompanionMechanicsInfo : public NodeInfo {
    CompanionMechanicsInfo() {
        nodeType = "Gameplay/CompanionMechanics";
        
        inputs = {
            "companionData",        // Companion data
            "environment",          // Environment data
            "characterData",        // Character data
            "companionState"        // Companion state
        };
        
        outputs = {
            "companionStatus",      // Companion status
            "companionMetrics"      // Companion performance metrics
        };
    }
};

class CompanionMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit CompanionMechanics(const CompanionMechanicsInfo& info = CompanionMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable companionData;    // Companion data, such as loyalty, bonding, etc.
    DataTable companionState;    // Companion state, such as loyalty, bonding, etc.
    int level = 0;
    float aggression = 0.0f;
    float protection = 0.0f;
    float loyalty = 0.0f;
    float bonding = 0.0f;
    float training = 0.0f;
    float damageBonus = 0.0f;   // Damage bonus for the companion.

    // === Processing ===
    void processNode() override { }
    void addCompanion(const std::string& companionName);
    void removeCompanion(const std::string& companionName);
    void processCompanion();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CompanionMechanics() = default;     // Default destructor
};

} // namespace hd

