/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Tessellator.h
 * @brief Header file for the Tessellator class.
 * 
 * ARCHITECTURAL NOTES:
 * - Tessellator is a class that represents a tessellation material in the engine's node graph.
 * - It is used to apply adaptive tessellation to meshes, meshlets.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Material.h"

namespace hd {

struct TessellatorInfo : public MaterialInfo {
    TessellatorInfo() {
        NodeType = "Rendering/Material/Tessellator";
        inputs = {
            "Mesh",   // Mesh to apply tessellation to
            "TessellationParams"  // Tessellation parameters
        };
        outputs = {
            "TessellatedMesh",  // Tessellation-applied mesh
            "TessellationMetrics"  // Tessellation performance metrics
        };
    }
};

class Tessellator : public Material {
public:
    // === Allocation, Initialization, Loading === 
    explicit Tessellator(const TessellatorInfo& info = TessellatorInfo())
        : Material(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Tessellator() = default;     // Default destructor
};

} // namespace hd
