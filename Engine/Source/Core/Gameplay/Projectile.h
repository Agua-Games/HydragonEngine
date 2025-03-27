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
            "model",              // Model of the projectile
            "position",           // Position of the projectile
            "rotation",           // Rotation of the projectile
            "scale",              // Scale of the projectile
            "velocity",           // Velocity of the projectile
            "material",           // Material of the projectile
            "energyMultiplier",   // Energy multiplier of the projectile
            "animation",          // Animation of the projectile
            "physics",            // Physics of the projectile
            "collision",          // Collision of the projectile
            "script"              // Script of the projectile
        };
        
        outputs = {
            "model",        // Model of the projectile
            "position",     // Position of the projectile
            "rotation",     // Rotation of the projectile
            "scale",        // Scale of the projectile
            "velocity",     // Velocity of the projectile
            "material",     // Material of the projectile
            "energyMultiplier",   // Energy multiplier of the projectile
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
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    Model model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    Material material;
    float energyMultiplier;
    Animation animation;
    Physics physics;
    Collision collision;
    Script script;
    
    // === Processing ===
    void processNode() override {
        model = getInputValue<Model>("model");
        position = getInputValue<glm::vec3>("position");
        rotation = getInputValue<glm::vec3>("rotation");
        scale = getInputValue<glm::vec3>("scale");
        velocity = getInputValue<glm::vec3>("velocity");
        material = getInputValue<Material>("material");
        energyMultiplier = getInputValue<float>("energyMultiplier");
        animation = getInputValue<Animation>("animation");
        physics = getInputValue<Physics>("physics");
        collision = getInputValue<Collision>("collision");
        script = getInputValue<Script>("script");

        // Process projectile
        auto projectileState = updateProjectile(model, position, rotation, scale, velocity, material, animation, physics, collision, script);
        
        // Set outputs
        setOutputValue("model", projectileState.model);
        setOutputValue("position", projectileState.position);
        setOutputValue("rotation", projectileState.rotation);
        setOutputValue("scale", projectileState.scale);
        setOutputValue("velocity", projectileState.velocity);
        setOutputValue("material", projectileState.material);
        setOutputValue("energyMultiplier", projectileState.energyMultiplier);
        setOutputValue("animation", projectileState.animation);
        setOutputValue("physics", projectileState.physics);
        setOutputValue("collision", projectileState.collision);
        setOutputValue("script", projectileState.script);

    }
    void update() override {}
    void processProjectile();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Projectile() = default;     // Default destructor
};

} // namespace hd


