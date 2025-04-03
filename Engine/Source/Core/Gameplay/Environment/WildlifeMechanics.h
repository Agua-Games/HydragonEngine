/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WildlifeMechanics.h
 * @brief Header file for the WildlifeMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - WildlifeMechanics is a class that represents wildlife mechanics in Hydragon.
 * - It is used to represent any wildlife mechanics in the game world, such as animal behavior, animal communication, animal objectives, etc.
 * - It supports interactive features, such as animal behavior, animal communication, animal objectives, etc. Also supports two-way messaging with other wildlife, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "DataTable.h"
#include "BehaviorTree.h"
#include "AgentAI.h"

namespace hd {
struct WildlifeMechanicsInfo : public BehaviorTreeInfo {
    WildlifeMechanicsInfo() {
        nodeType = "Gameplay/WildlifeMechanics";
        
        inputs = {
            "wildlifeMechanicsData",// Wildlife mechanics data
            "environment",          // Environment data
            "characterData",        // Character data
            "wildlifeMechanicsState"// Wildlife mechanics state
        };
        
        outputs = {
            "wildlifeMechanicsStatus",// Wildlife mechanics status
            "wildlifeMechanicsMetrics"// Wildlife mechanics performance metrics
        };
    }
};

class WildlifeMechanics : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit WildlifeMechanics(const WildlifeMechanicsInfo& info = WildlifeMechanicsInfo())
        : BehaviorTree(info) {}    // Initialize the BehaviorTree with the provided info structure
    initialize() override {}
    load() override {}

    // Set default values
    float interactionRadius = 0.0f;
    bool enableMigration = false;

    // === Processing ===
    void processNode() override { }
    void addSpecies(const std::string& speciesName);
    void removeSpecies(const std::string& speciesName);
    void processWildlifeMechanics();     // Process wildlife mechanics logic
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WildlifeMechanics() = default;     // Default destructor
};

} // namespace hd