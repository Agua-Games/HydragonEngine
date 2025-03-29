/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Jetpack.h
 * @brief Header file for the Jetpack class.
 * 
 * ARCHITECTURAL NOTES:
 * - Jetpack is a class that represents a jetpack in Hydragon.
 * - It is used to represent any jetpack in the game world, such as backpacks, vests, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other jetpacks, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct JetpackInfo : public NodeInfo {
    JetpackInfo() {
        nodeType = "Gameplay/Jetpack";
        
        inputs = {
            "jetpackType",        // Type of jetpack (backpack, vest, etc.)
            "jetpackData",        // Jetpack data
            "environment",        // Environment data
            "characterData",      // Character data
            "jetpackState"        // Jetpack state
        };
        
        outputs = {
            "jetpackStatus",      // Jetpack status
            "jetpackMetrics"      // Jetpack performance metrics
        };
    }
};

class Jetpack : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Jetpack(const JetpackInfo& info = JetpackInfo())
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
    ~Jetpack() = default;     // Default destructor
};

} // namespace hd
