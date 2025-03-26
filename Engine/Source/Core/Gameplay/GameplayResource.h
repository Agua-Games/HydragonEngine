/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GameplayResource.h
 * @brief Header file for the GameplayResource class.
 * 
 * ARCHITECTURAL NOTES:
 * - GameplayResource is a class that represents a gameplay resource in Hydragon.
 * - It is used to represent any gameplay resource in the game world, such as items, weapons, vehicles, etc.
 * - It supports interactive features, such as using the resource, interacting with the environment, etc. Also supports two-way messaging with other gameplay resources, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct GameplayResourceInfo : public NodeInfo {
    GameplayResourceInfo() {
        nodeType = "Gameplay/GameplayResource";
        
        inputs = {
            "resourceType",        // Type of gameplay resource (item, weapon, vehicle, etc.)
            "resourceData",        // Gameplay resource data
            "environment",         // Environment data
            "characterData",       // Character data
            "resourceState"        // Gameplay resource state
        };
        
        outputs = {
            "resourceStatus",      // Gameplay resource status
            "resourceMetrics"      // Gameplay resource performance metrics
        };
    }
};

class GameplayResource : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit GameplayResource(const GameplayResourceInfo& info = GameplayResourceInfo())
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
    ~GameplayResource() = default;     // Default destructor
};

} // namespace hd
