/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Wildlife.h
 * @brief Header file for the Wildlife class.
 * 
 * ARCHITECTURAL NOTES:
 * - Wildlife is a class that represents wildlife in Hydragon.
 * - It is used to represent any wildlife in the game world, such as animals, birds, insects, etc.
 * - Think of Wildlife, as a collection of animals, birds, insects, etc. It's useful to model fauna specific to a given biome. e.g. you create a forest using Vegetation class,
 * or Forest, procedurally, using rulesets for spawning, placement, over a terrain, then you can create a Wildlife node, and add it to the forest, to represent the fauna specific 
 * to that biome. Then, WildlifeMechanics can be used to give dynamic behaviors, interactions, to those species.
 * - So, the Wildlife class is a container for many species populating an area, and the Population node can be used to represent a population of a specific species. Then,
 * WildlifeMechanics can be used to give dynamic behaviors, interactions, to those species. To sum up the nodes setup, hierarchy:
 *      
 *      WildlifeManager --> WildlifeMechanics --> Wildlife (contains array of populations) --> Population --> Individual animal/bird/insect
 * 
 * When we mention that Wildlife is related to a biome, this is not a strict thing - it can be something like an area in a level, or a biome in a world. e.g. an outpost atop
 * a plateau, and the Wildlife is the fauna specific to that area, like some bird species, some insect species, etc.
 * - It supports interactive features, such as animal behavior, animal navigation habits throughout the level/world along the day, animal communication, animal objectives, etc. 
 * Also supports two-way messaging with other wildlife, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "WildlifeMechanics.h"

namespace hd {

struct WildlifeInfo : public NodeInfo {
    WildlifeInfo() {
        nodeType = "Gameplay/Wildlife";
        
        inputs = {
            "wildlifeData",        // Wildlife data
            "environment",         // Environment data
            "characterData",       // Character data
            "wildlifeState"        // Wildlife state
        };
        
        outputs = {
            "wildlifeStatus",      // Wildlife status
            "wildlifeMetrics"      // Wildlife performance metrics
        };
    }
};

class Wildlife : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Wildlife(const WildlifeInfo& info = WildlifeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addSpecies(const std::string& speciesName);
    void removeSpecies(const std::string& speciesName);
    void processWildlife();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Wildlife() = default;     // Default destructor
};

} // namespace hd
