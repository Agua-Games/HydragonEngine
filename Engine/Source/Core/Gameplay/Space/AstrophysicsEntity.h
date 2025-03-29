/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AstrophysicsEntity.h
 * @brief Header file for the AstrophysicsEntity class.
 * 
 * ARCHITECTURAL NOTES:
 * - AstrophysicsEntity is a class that represents an astrophysics entity in Hydragon.
 * - It is used to represent any astrophysics entity in the game world, such as stars, planets, moons, asteroids, etc.
 * - It supports interactive features, such as orbiting, rotating, and exploding. Also supports two-way messaging with other astrophysics entities, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct AstrophysicsEntityInfo : public NodeInfo {
    AstrophysicsEntityInfo() {
        nodeType = "Gameplay/AstrophysicsEntity";
        
        inputs = {
            "entityType",        // Type of astrophysics entity (star, planet, moon, asteroid, etc.)
            "entityData",        // Astrophysics entity data
            "environment",       // Environment data
            "characterData",     // Character data
            "entityState"        // Astrophysics entity state
        };
        
        outputs = {
            "entityStatus",      // Astrophysics entity status
            "entityMetrics"      // Astrophysics entity performance metrics
        };
    }
};

class AstrophysicsEntity : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AstrophysicsEntity(const AstrophysicsEntityInfo& info = AstrophysicsEntityInfo())
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
    ~AstrophysicsEntity() = default;     // Default destructor
};

} // namespace hd
