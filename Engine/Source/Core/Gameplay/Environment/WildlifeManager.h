/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WildlifeManager.h
 * @brief Header file for the WildlifeManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - WildlifeManager is a singleton class that manages the wildlife in the game.
 * - It is responsible for managing the wildlife in the game.
 * - It supports interactive features, such as animal behavior, animal communication, animal objectives, etc. Also supports two-way messaging with other wildlife, environment, character, etc.
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
struct WildlifeManagerInfo : public NodeInfo {
    WildlifeManagerInfo() {
        nodeType = "Gameplay/WildlifeManager";
        
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

class WildlifeManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit WildlifeManager(const WildlifeManagerInfo& info = WildlifeManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addWildlife(const std::string& wildlifeName);
    void removeWildlife(const std::string& wildlifeName);
    void processWildlife();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WildlifeManager() = default;     // Default destructor
};

} // namespace hd

