/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Projectile.h
 * @brief Header file for the Projectile class.
 * 
 * ARCHITECTURAL NOTES:
 * - Projectile is a class that represents a projectile in Hydragon.
 * - It is used to represent any projectile in the game world, such as bullets, arrows, etc.
 * - It supports interactive features, such as firing, aiming, and colliding. Also supports two-way messaging with other projectiles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Prop.h"

namespace hd {

struct ProjectileInfo : public NodeInfo {
    ProjectileInfo() {
        nodeType = "Gameplay/Projectile";
        
        inputs = {
            "model",        // Model of the projectile
            "position",     // Position of the projectile
            "rotation",     // Rotation of the projectile
            "scale",        // Scale of the projectile
            "material",     // Material of the projectile
            "animation",    // Animation of the projectile
            "physics",      // Physics of the projectile
            "collision",    // Collision of the projectile
            "script"        // Script of the projectile
        };
        
        outputs = {
            "model",        // Model of the projectile
            "position",     // Position of the projectile
            "rotation",     // Rotation of the projectile
            "scale",        // Scale of the projectile
            "material",     // Material of the projectile
            "animation",    // Animation of the projectile
            "physics",      // Physics of the projectile
            "collision",    // Collision of the projectile
            "script"        // Script of the projectile
        };
    }
};

class Projectile : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Projectile(const ProjectileInfo& info = ProjectileInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {
 
    }
    void update() override {}
    void processProjectile();
    void () override {
        processProjectile(); 
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Projectile() = default;     // Default destructor
};

} // namespace hd


