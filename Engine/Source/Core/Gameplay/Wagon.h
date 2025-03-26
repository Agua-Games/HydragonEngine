/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Wagon.h
 * @brief Header file for the Wagon class.
 * 
 * ARCHITECTURAL NOTES:
 * - Wagon is a class that represents a wagon in Hydragon.
 * - It is used to represent any wagon in the game world, such as carts, wagons, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other wagons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct WagonInfo : public NodeInfo {
    WagonInfo() {
        nodeType = "Gameplay/Wagon";
        
        inputs = {
            "wagonType",        // Type of wagon (cart, wagon, etc.)
            "wagonData",        // Wagon data
            "environment",      // Environment data
            "characterData",    // Character data
            "wagonState"        // Wagon state
        };
        
        outputs = {
            "wagonStatus",      // Wagon status
            "wagonMetrics"      // Wagon performance metrics
        };
    }
};

class Wagon : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Wagon(const WagonInfo& info = WagonInfo())
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
    ~Wagon() = default;     // Default destructor
};

} // namespace hd