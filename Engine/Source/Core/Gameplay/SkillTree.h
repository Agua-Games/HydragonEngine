/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SkillTree.h
 * @brief Header file for the SkillTree class.
 * 
 * ARCHITECTURAL NOTES:
 * - SkillTree is a class that represents a skill tree in Hydragon.
 * - It is used to represent and process skill trees.
 * - It uses the Vulkan API for skill tree processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SkillTreeInfo : public NodeInfo {
    SkillTreeInfo() {
        nodeType = "Gameplay/SkillTree";
        
        inputs = {
            "skillData",       // Skill data
            "characterData",   // Character data
            "skillState"       // Skill state
        };
        
        outputs = {
            "skillStatus",     // Skill status
            "skillMetrics"     // Skill performance metrics
        };
    }
};

class SkillTree : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SkillTree(const SkillTreeInfo& info = SkillTreeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addSkill(const std::string& skillName);
    void removeSkill(const std::string& skillName);
    void processSkill();
    void processSkillTree();
    void computeResult();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SkillTree() = default;     // Default destructor
};

} // namespace hd
