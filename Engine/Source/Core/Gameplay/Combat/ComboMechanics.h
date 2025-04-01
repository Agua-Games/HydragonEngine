/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ComboMechanics.h
 * @brief Header file for the ComboMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - ComboMechanics is a class that represents combo mechanics in Hydragon.
 * - It is used to represent any combo mechanics in the game world, such as combo chains, combo points, etc.
 * - It supports interactive features, such as combo chains, combo points, and combo bonuses. Also supports two-way messaging with other combo mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Ability.h"

namespace hd {

struct ComboMechanicsInfo : public NodeInfo {
    ComboMechanicsInfo() {
        nodeType = "Gameplay/ComboMechanics";
        
        inputs = {
            "comboData",        // Combo data
            "environment",      // Environment data
            "characterData",    // Character data
            "comboState"        // Combo state
        };
        
        outputs = {
            "comboStatus",      // Combo status
            "comboMetrics"      // Combo performance metrics
        };
    }
};

class ComboMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ComboMechanics(const ComboMechanicsInfo& info = ComboMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float multiplier = 1.0f;
    std::string condition = "none";
    float points = 0.0f;
    float bonus = 0.0f;
    float threshold = 0.0f;
    float cooldown = 0.0f;
    float regeneration = 0.0f;
    std::vector<Ability> comboAbilities;
    DataTable comboMechanicsData;

    // === Processing ===
    void processNode() override { }
    void addCombo(const std::string& comboName);
    void removeCombo(const std::string& comboName);
    void processCombo();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ComboMechanics() = default;     // Default destructor
};

} // namespace hd
