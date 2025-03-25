/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file CatmullClarkSubdiv.h
 * @brief Header file for the CatmullClarkSubdiv class.
 * 
 * ARCHITECTURAL NOTES:
 * - CatmullClarkSubdiv is a class that represents a Catmull-Clark subdivision material in the engine's node graph.
 * - It is used to apply Catmull-Clark subdivision to meshes.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Material.h"

namespace hd {

struct CatmullClarkSubdivInfo : public MaterialInfo {
    CatmullClarkSubdivInfo() {
        NodeType = "Rendering/Material/CatmullClarkSubdiv";
        inputs = {
            "Mesh",   // Mesh to apply Catmull-Clark subdivision to
            "SubdivParams"  // Subdivision parameters
        };
        outputs = {
            "SubdivMesh",  // Subdivision-applied mesh
            "SubdivMetrics"  // Subdivision performance metrics
        };
    }
};

class CatmullClarkSubdiv : public Material {
public:
    // === Allocation, Initialization, Loading === 
    explicit CatmullClarkSubdiv(const CatmullClarkSubdivInfo& info = CatmullClarkSubdivInfo())
        : Material(info) {}   
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
    ~CatmullClarkSubdiv() = default;     // Default destructor
};

} // namespace hd
