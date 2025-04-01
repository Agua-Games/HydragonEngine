/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Skill.h
 * @brief Header file for the Skill class.
 * 
 * ARCHITECTURAL NOTES:
 * - Skill is a class that represents a skill in Hydragon. Primarily used for the skill trees.
 * - It is used to represent any skill in the game world, such as abilities, powers, spells, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other skills, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

enum class SkillType {
    ability,
    power,
    spell,
    melee,
    ranged,
    custom
};

struct SkillInfo : public NodeInfo {
    SkillInfo() {
        nodeType = "Gameplay/Skill";
        
        inputs = {
            "skillType",       // Type of skill (ability, power, spell, etc.)
            "skillData",       // Skill data
            "environment",     // Environment data
            "characterData",   // Character data
            "skillState"       // Skill state
        };
        
        outputs = {
            "skillStatus",     // Skill status
            "skillMetrics"     // Skill performance metrics
        };
    }
};

class Skill : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Skill(const SkillInfo& info = SkillInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    SkillInfo skillInfo = {};  // Initialize skill info struct with default values
    SkillType type = SkillType::custom;
    float cooldown = 0.0f;
    int level = 0;
    int requiredLevel = 0;
     std::unordered_map<std::string, int> requiredStat = {};
    DataTable skillData = DataTable();

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Skill() = default;     // Default destructor
};

} // namespace hd
