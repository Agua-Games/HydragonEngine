/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SurvivalMechanics.h
 * @brief Header file for the SurvivalMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - SurvivalMechanics is a class that represents survival mechanics in Hydragon.
 * - It is used to represent any survival mechanics in the game world, such as hunger, thirst, etc.
 * - It supports interactive features, such as eating, drinking, and resting. Also supports two-way messaging with other survival mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SurvivalMechanicsInfo : public NodeInfo {
    SurvivalMechanicsInfo() {
        nodeType = "Gameplay/SurvivalMechanics";
        
        inputs = {
            "survivalData",        // Survival data
            "environment",         // Environment data
            "characterData",       // Character data
            "survivalState"        // Survival state
        };
        
        outputs = {
            "survivalStatus",      // Survival status
            "survivalMetrics"      // Survival performance metrics
        };
    }
};

class SurvivalMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit SurvivalMechanics(const SurvivalMechanicsInfo& info = SurvivalMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SurvivalMechanics() = default;     // Default destructor
};

} // namespace hd
