/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ExperienceTree.h
 * @brief Header file for the ExperienceTree class.
 * 
 * ARCHITECTURAL NOTES:
 * - ExperienceTree is a class that represents an experience tree in Hydragon.
 * - It is used to represent and process experience trees.
 * - It uses the Vulkan API for experience tree processing.
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

struct ExperienceTreeInfo : public NodeInfo {
    ExperienceTreeInfo() {
        nodeType = "Gameplay/ExperienceTree";
        
        inputs = {
            "xpData",        // Experience data
            "characterData", // Character data
            "xpState"        // Experience state
        };
        
        outputs = {
            "xpStatus",      // Experience status
            "xpMetrics"      // Experience performance metrics
        };
    }
};

class ExperienceTree : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ExperienceTree(const ExperienceTreeInfo& info = ExperienceTreeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addXP(const std::string& xpName);
    void removeXP(const std::string& xpName);
    void addXPSource(const std::string& xpSourceName, float xpSourceValue);
    void removeXPSource(const std::string& xpSourceName);
    void processXP();
    void processXPtree();
    void computeResult();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ExperienceTree() = default;     // Default destructor
};

} // namespace hd
