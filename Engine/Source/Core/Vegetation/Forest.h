/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Forest.h
 * @brief Header file for the Forest class.
 * 
 * ARCHITECTURAL NOTES:
 * - Forest is a class that represents a forest in Hydragon.
 * - It is used to represent any forest in the game world, such as deciduous forests, coniferous forests, etc.
 * - It is more of a convenience class, derived from LayeredPlacement, with some other functionality, like riverlets, etc. You can achieve a forest using other available
 * nodes, but this class provides a more convenient way.
 * - It supports procedural features, such as tree generation, tree growth, and tree death. Also supports two-way messaging with other forests, environment, character, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other forests, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vegetation.h"
#include "EnvironmentManager.h"

namespace hd {

struct ForestInfo : public VegetationInfo {
    ForestInfo() {
        nodeType = "Gameplay/Forest";
        
        inputs = {
            "forestType",       // Type of forest (deciduous, coniferous, etc.)
            "forestData",       // Forest data
            "environment",      // Environment data
            "characterData",    // Character data
            "forestState"       // Forest state
        };
        
        outputs = {
            "forestStatus",     // Forest status
            "forestMetrics"     // Forest performance metrics
        };
    }
};

class Forest : public Vegetation {
public:
    // === Allocation, Initialization, Loading ===
    explicit Forest(const ForestInfo& info = ForestInfo())
        : Vegetation(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override { }
    void addPlant(const Vegetation& plant);
    void removePlant(const Vegetation& plant);
    void addTree(const Vegetation& tree);
    void removeTree(const Vegetation& tree);
    void processPlants();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Forest() = default;     // Default destructor
};

}