/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SolidShape.h
 * @brief Header file for the SolidShape class.
 * 
 * ARCHITECTURAL NOTES:
 * - SolidShape is a class that represents a solid shape in Hydragon. SolidShape is physics-enabled, via WavePhysics.
 * - SolidShape supports traditional rigid body joints and constraints via logical links, like ParentLink, PointLink, etc.
 * - It is used to represent any solid shape in the game world, such as boxes, spheres, cylinders, etc.
 * - It also support procedurally generated shapes and input meshes from assets.
 * - It supports interactive features, such as collision detection and response.
 * - It uses the Vulkan API for shape processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct SolidShapeInfo : public NodeInfo {
    SolidShapeInfo() {
        NodeType = "Physics/SolidShape";
        
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

class SolidShape : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SolidShape(const SolidShapeInfo& info = SolidShapeInfo())
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
    ~SolidShape() = default;     // Default destructor
};

} // namespace hd
