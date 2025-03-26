/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EM.h
 * @brief represents a physics EM body node in the engine's node graph. It is physics-enabled, via WavePhysics.
 * 
 * ARCHITECTURAL NOTES:
 * - EM holds one or more EM fields and supports EM simulation and seamless interaction with other physics entities, like particles, solids, etc.
 * - It is used  in general to represent fields such as EM field, within a shape/container in the game world, 
 * such as boxes, spheres, cylinders, custom meshes, etc.
 * - It supports procedurally generated shapes and input meshes from assets.
 */
#pragma once
#include "Node.h"
#include "Wave.h"
#include "WavePhysics.h"
#include "PhysicsFields.h"
#include "ProceduralTypes.h"
#include "RuntimeVariants.h"

namespace hd {

struct EMInfo : public NodeInfo {
    EMInfo() {
        NodeType = "Physics/EM";
        
        inputs = {
            "ShapeType",       // Type of the shape
            "Dimensions",      // Dimensions of the shape
            "Material",        // Material properties
            "CollisionMesh",   // Collision mesh
            "PhysicsProperties"// Physics properties
        };
        
        outputs = {
            "ShapeData",       // Shape data
            "CollisionData",   // Collision data
            "PhysicsData"      // Physics data
        };
    }
};

class EM : public Node {
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
    explicit EM(const EMInfo& info = EMInfo())
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
    ~EM() = default;     // Default destructor
};

} // namespace hd
