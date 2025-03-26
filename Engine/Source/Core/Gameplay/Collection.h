/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Collection.h
 * @brief Header file for the Collection class.
 * 
 * ARCHITECTURAL NOTES:
 * - Collection is a class that represents a collection in Hydragon.
 * - It is used to represent any collection in the game world, such as groups, teams, etc.
 * - It supports interactive features, such as adding, removing, and iterating over. Also supports two-way messaging with other collections, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CollectionInfo : public NodeInfo {
    CollectionInfo() {
        nodeType = "Gameplay/Collection";
        
        inputs = {
            "collectionType",  // Type of collection (group, team, etc.)
            "collectionData",  // Collection data
            "environment",     // Environment data
            "characterData",   // Character data
            "collectionState"  // Collection state
        };
        
        outputs = {
            "collectionStatus",  // Collection status
            "collectionMetrics"  // Collection performance metrics
        };
    }
};

class Collection : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Collection(const CollectionInfo& info = CollectionInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Collection() = default;     // Default destructor
};

} // namespace hd
