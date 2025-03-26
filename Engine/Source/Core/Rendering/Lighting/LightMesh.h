/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LightMesh.h
 * @brief Header file for the LightMesh class.
 * 
 * ARCHITECTURAL NOTES:
 * - LightMesh is a node that represents a light that emits from a mesh, and is rendered as a mesh.
 * - It has support for built-in primitives and can also accept a mesh asset as input.
 * - It can be used to create different types of lights with various properties.
 * - It uses the Vulkan API for light management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Node.h"
#include "Light.h"

namespace hd {

struct LightMeshInfo : public LightInfo {
    LightMeshInfo() {
        NodeType = "Rendering/LightMesh";
        inputs.insert(inputs.end(), {
            "MeshAsset",       // Mesh asset for the light
            "PrimitiveType",   // Built-in primitive type
            "Scale",           // Scale of the light mesh
            "Color",           // Color of the light mesh
            "Intensity"        // Intensity of the light mesh
        });
        
        outputs.insert(outputs.end(), {
            "MeshData",        // Mesh data for rendering
            "LightData"        // Light data for rendering
        });
    }
};

class LightMesh : public Light {
public:
    // === Allocation, Initialization, Loading ===
    explicit LightMesh(const LightMeshInfo& info = LightMeshInfo())
        : Light(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LightMesh() = default;     // Default destructor
};

} // namespace hd
