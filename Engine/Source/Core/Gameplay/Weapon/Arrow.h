/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Arrow.h
 * @brief Header file for the Arrow class.
 * 
 * ARCHITECTURAL NOTES:
 * - Arrow is a class that represents an arrow in Hydragon.
 * - It is used to represent any arrow in the game world, such as arrows, bolts, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other arrows, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Projectile.h"

namespace hd {

struct ArrowInfo : public ProjectileInfo {
    ArrowInfo() {
        nodeType = "Gameplay/Arrow";
        
        inputs = {
            "arrowType",  // Type of arrow (arrow, bolt, etc.)
            "arrowData",  // Arrow data
            "environment", // Environment data
            "characterData", // Character data
            "arrowState" // Arrow state
        };
        
        outputs = {
            "arrowStatus", // Arrow status
            "arrowMetrics" // Arrow performance metrics
        };
    }
};

class Arrow : public Projectile {
public:
    // === Allocation, Initialization, Loading ===
    explicit Arrow(const ArrowInfo& info = ArrowInfo())
        : Projectile(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void fire();
    void reload();
    void aim();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Arrow() = default;     // Default destructor
};

} // namespace hd

