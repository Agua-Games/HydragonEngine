/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file RigidBody.h
 * @brief Header file for the RigidBody class.
 * 
 * ARCHITECTURAL NOTES:
 * - RigidBody is a class that represents a rigid body in Hydragon.
 * - It is based on traditional rigid body physics, uses rigid body dynamics and we make our best efforts to keep it compatible with WavePhysics, at least
 * until we validate WavePhysics and its solidField approach's performance and solidity (no pun intended). So, stay informed that this may become a legacy class.
 * - It is used to represent any rigid body in the game world, such as boxes, spheres, cylinders, etc.
 * - It supports interactive features, such as collision detection and response.
 * - It uses the Vulkan API for shape processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "WavePhysics.h"
#include "PhysicsTypes.h"             // From WavePhysics, for the body state, collision events, forces, energy state, etc.
#include "PhysicsFields.h"            // From WavePhysics, for the solidField, fluidField, etc.
#include "ProceduralTypes.h"          // From WavePhysics, for the procedural shape types, such as Box, Sphere, Cylinder, etc.

namespace hd {

struct RigidBodyInfo : public NodeInfo {
    RigidBodyInfo() {
        NodeType = "Physics/RigidBody";
        
        inputs = {
            "ShapeType",            // Type of the shape
            "solidShape",           // Rigid body shape
            "Material",             // Material properties
            "solidShapeMesh",       // Collision mesh
            "PhysicsProperties",
            "InitialPosition",      // Initial position
            "InitialVelocity"       // Initial velocity
        };
        
        outputs = {
            "BodyState",            // Current body state
            "CollisionEvents",      // Collision events
            "Forces",               // Applied forces
            "EnergyState"           // Energy state
        };
    }
};

class RigidBody : public Node {
public:
    // === Structure Definitions ===
    enum class ShapeType {
        Box,
        Sphere,
        Cylinder,
        Mesh,
        Custom
    };

    // === Allocation, Initialization, Loading ===
    explicit RigidBody(const RigidBodyInfo& info = RigidBodyInfo())
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
    ~RigidBody() = default;     // Default destructor
};

} // namespace hd
