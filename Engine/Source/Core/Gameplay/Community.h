/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Community.h
 * @brief Header file for the Community class.
 * 
 * ARCHITECTURAL NOTES:
 * - Community is a class that represents a community in Hydragon.
 * - It is used to represent any community in the game world, such as player communities, enemy communities, etc.
 * - It supports interactive features, such as community management, community communication, community objectives, etc. Also supports two-way messaging with other communities, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CommunityInfo : public NodeInfo {
    CommunityInfo() {
        nodeType = "Gameplay/Community";
        
        inputs = {
            "communityType",   // Type of community (player, enemy, etc.)
            "communityData",   // Community data
            "environment",     // Environment data
            "characterData",   // Character data
            "communityState"   // Community state
        };
        
        outputs = {
            "communityStatus", // Community status
            "communityMetrics" // Community performance metrics
        };
    }
};

class Community : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Community(const CommunityInfo& info = CommunityInfo())
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
    ~Community() = default;     // Default destructor
};

} // namespace hd