/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Beast.h
 * @brief Header file for the Beast class.
 * 
 * ARCHITECTURAL NOTES:
 * - Beast is a class that represents a beast in Hydragon.
 * - It is used to represent any beast in the game world, such as lions, tigers, bears, etc.
 * - It supports interactive features, such as roaring, growling, and biting. Also supports two-way messaging with other beasts, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct BeastInfo : public NodeInfo {
    BeastInfo() {
        nodeType = "Gameplay/Beast";
        
        inputs = {
            "beastType",        // Type of beast (lion, tiger, bear, etc.)
            "beastData",        // Beast data
            "environment",      // Environment data
            "characterData",    // Character data
            "beastState"        // Beast state
        };
        
        outputs = {
            "beastStatus",      // Beast status
            "beastMetrics"      // Beast performance metrics
        };
    }
};

class Beast : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Beast(const BeastInfo& info = BeastInfo())
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
    ~Beast() = default;     // Default destructor
};

} // namespace hd
