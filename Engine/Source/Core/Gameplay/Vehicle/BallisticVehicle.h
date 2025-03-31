/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BallisticVehicle.h
 * @brief Header file for the BallisticVehicle class.
 * 
 * ARCHITECTURAL NOTES:
 * - BallisticVehicle is a class that represents a ballistic vehicle in Hydragon.
 * - It is used to represent any ballistic vehicle in the game world, such as rockets, missiles, etc.
 * - It supports interactive features, such as launching, steering, and exploding. Also supports two-way messaging with other ballistic vehicles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct BallisticVehicleInfo : public VehicleInfo {
    BallisticVehicleInfo() {
        nodeType = "Gameplay/BallisticVehicle";
        
        inputs = {
            "model",        // Model of the ballistic vehicle
            "position",     // Position of the ballistic vehicle
            "rotation",     // Rotation of the ballistic vehicle
            "scale",        // Scale of the ballistic vehicle
            "material",     // Material of the ballistic vehicle
            "animation",    // Animation of the ballistic vehicle
            "physics",      // Physics of the ballistic vehicle
            "collision",    // Collision of the ballistic vehicle
            "script"        // Script of the ballistic vehicle
        };
        
        outputs = {
            "model",        // Model of the ballistic vehicle
            "position",     // Position of the ballistic vehicle
            "rotation",     // Rotation of the ballistic vehicle
            "scale",        // Scale of the ballistic vehicle
            "material",     // Material of the ballistic vehicle
            "animation",    // Animation of the ballistic vehicle
            "physics",      // Physics of the ballistic vehicle
            "collision",    // Collision of the ballistic vehicle
            "script"        // Script of the ballistic vehicle
        };
    }
};

class BallisticVehicle : public Vehicle {
public:
    // === Structure Definitions ===
    enum PayloadType {
        HighExplosive,
        Nuclear,
        Incendiary,
        Fragmentation,
        Other
    };
    
    enum GuidanceType {
        Inertial,
        TerrainFollowing,
        TerrainAvoidance,
        Other
    };
    
    enum EvasionType {
        None,
        Active,
        Passive,
        Other
    };

    enum SonicType {
        Subsonic,
        Supersonic,
        Hypersonic,
        Other
    };

    // === Allocation, Initialization, Loading ===
    explicit BallisticVehicle(const BallisticVehicleInfo& info = BallisticVehicleInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    vec3 velocity = vec3(0.0f);
    float range = 0.0f;
    float speed = 0.0f;
    float damage = 0.0f;
    float penetration = 0.0f;
    float guidance = 0.0f;
    float guidanceRange = 0.0f;
    float explosionRadius = 0.0f;
    float explosionDamage = 0.0f;
    float explosionForce = 0.0f;
    float explosionDuration = 0.0f;
    float explosionCooldown = 0.0f;
    PayloadType payload = PayloadType::Other;
    GuidanceType guidanceType = GuidanceType::Other;
    EvasionType evasionType = EvasionType::Other;
    SonicType sonicType = SonicType::Other;


    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BallisticVehicle() = default;     // Default destructor
};

} // namespace hd
