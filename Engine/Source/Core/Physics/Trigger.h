/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file Trigger.h
 * @brief Header file for the Trigger class.
 * 
 * ARCHITECTURAL NOTES:
 * - Trigger is a class that represents a trigger in Hydragon.
 * - It is used to represent any trigger in the game world, such as collision zones, sensor zones, etc.
 * - It is physics-enabled, modeled using solidField shapes, via WavePhysics. The difference being that it doesn't have any mass, inertia, etc.
 * Nor it participate in any physical simulation or Energy Transfer event, just detects collisions.
 * - It also support procedurally generated shapes and input meshes from assets.
 * - It supports interactive features, such as collision detection and response.
 * - It uses the Vulkan API for shape processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct TriggerInfo : public NodeInfo {
    TriggerInfo() {
        NodeType = "Physics/Trigger";
        
        inputs = {
            "Shape",            // Trigger shape
            "Material",        // Material properties
            "CollisionMesh",   // Collision mesh
            "PhysicsProperties"// Physics properties
        };
        
        outputs = {
            "TriggerData",     // Trigger data
            "CollisionData",   // Collision data
            "PhysicsData"      // Physics data
        };
    }
};

class Trigger : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Trigger(const TriggerInfo& info = TriggerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Trigger() = default;     // Default destructor
};

} // namespace hd
