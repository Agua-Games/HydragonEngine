/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StatModifier.h
 * @brief Header file for the StatModifier class.
 * 
 * ARCHITECTURAL NOTES:
 * - StatModifier is a class that represents a stat modifier in Hydragon.
 * - It is used to modify the stats of characters, vehicles, etc, in the game world.
 * - It supports various types of stat modifications, such as health, mana, movement, etc.
 * - It supports interactive features, such as modifying, updating, and accessing. Also supports two-way messaging with other stat modifiers, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct StatModifierInfo : public NodeInfo {
    StatModifierInfo() {
        nodeType = "Gameplay/StatModifier";
        
        inputs = {
            "statType",       // Type of stat (health, mana, movement, etc.)
            "statData",       // Stat data
            "environment",    // Environment data
            "characterData",  // Character data
            "statModifier"    // Stat modifier data
        };
        
        outputs = {
            "statStatus",     // Stat status
            "statMetrics"     // Stat performance metrics
        };
    }
};

class StatModifier : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit StatModifier(const StatModifierInfo& info = StatModifierInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float value = 0.0f;
    float threshold = 0.0f;
    float duration = 0.0f;
    float tickRate = 0.0f;

    // === Processing ===
    void processNode() override {
    }
    void addStat(const std::string& statName, float statValue);
    void modifyStat();
    void removeStat(const std::string& statName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~StatModifier() = default;     // Default destructor
};

} // namespace hd
