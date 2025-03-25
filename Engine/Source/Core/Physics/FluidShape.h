/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FluidShape.h
 * @brief Header file for the FluidShape class.
 * 
 * ARCHITECTURAL NOTES:
 * - FluidShape is a class that represents a fluid shape in Hydragon.
 * - It is used to represent any fluid shape in the game world, such as water, oil, gas, etc.
 * - It also support procedurally generated shapes and input meshes from assets.
 * - It supports interactive features, such as collision detection and response.
 * - It uses the Vulkan API for shape processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct FluidShapeInfo : public NodeInfo {
    FluidShapeInfo() {
        NodeType = "Physics/FluidShape";
        
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

class FluidShape : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit FluidShape(const FluidShapeInfo& info = FluidShapeInfo())
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
    ~FluidShape() = default;     // Default destructor
};

} // namespace hd
