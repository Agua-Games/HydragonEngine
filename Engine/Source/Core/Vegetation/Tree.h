/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Tree.h
 * @brief Header file for the Tree class.
 * 
 * ARCHITECTURAL NOTES:
 * - Tree is a class that represents a tree in Hydragon.
 * - It is used to represent any tree in the game world, such as deciduous trees, coniferous trees, etc.
 * - It derives from Vegetation, providing more specific features for trees, like climbing, cracking/destructible, fruit collectables, bird nests, poles, etc.
 * - The vegetation subsystem provides built-in LOD support, adaptive switching from basic trees to full featured hero trees, etc.
 * - It inherits and enhances procedural features (generation, deformation, growth, death) from Vegetation.
 * - It supports procedural features, such as tree generation, tree growth, and tree death. Also supports two-way messaging with other trees, environment, character, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other trees, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vegetation.h"

namespace hd {

struct TreeInfo : public VegetationInfo {
    TreeInfo() {
        nodeType = "Vegetation/Tree";  
        
        inputs = {
            "treeType",        // Type of tree (deciduous, coniferous, etc.)
            "treeData",        // Tree data
            "environment",     // Environment data
            "characterData",   // Character data
            "treeState"        // Tree state
        };
        
        outputs = {
            "treeStatus",      // Tree status
            "treeMetrics"      // Tree performance metrics
        };
    }
};

class Tree : public Vegetation {
public:
    // === Allocation, Initialization, Loading ===
    explicit Tree(const TreeInfo& info = TreeInfo())
        : Vegetation(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processTree();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Tree() = default;     // Default destructor
};

} // namespace hd
