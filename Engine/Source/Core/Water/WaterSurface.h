/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WaterSurface.h
 * @brief Header file for the WaterSurface class.
 * 
 * ARCHITECTURAL NOTES:
 * - WaterSurface is a class that represents a water surface node in the engine's node graph.
 * - It is used to represent and process water surfaces, like ponds, lakes, oceans, etc.
 * - It supports various water surface types, such as standing waves, moving waves, etc.
 * - It supports procedural features, such as wave generation, wave movement, wave reflection, wave refraction, etc.
 * - It leverages dedicated water shaders, an adaptive LOD system which makes use of tiles, tessellation, and other techniques.
 * - It supports underwater fullscreen post-processing, like caustics, godrays, lens flares, lens blur, etc.
 * - It uses the Vulkan API for water surface processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "ProceduralTypes.h"

namespace hd {

struct WaterSurfaceInfo : public NodeInfo {
    WaterSurfaceInfo() {
        NodeType = "Water/WaterSurface";
        
        inputs = {
            "WaterType",        // Type of water surface (pond, lake, ocean, etc.)
            "WaterParams",      // Water surface parameters
            "WaterState",       // Water surface state
            "WaterFlow",        // Water flow parameters
            "WaterInteraction", // Water interaction parameters
            "WaterRendering",   // Water rendering parameters
            "WaterPhysics",     // Water physics parameters
            "WaterLOD",         // Water LOD parameters
            "WaterLODState"     // Water LOD state
        };
        
        outputs = {
            "WaterLODState",    // Water LOD state
            "WaterLODMetrics"   // Water LOD performance metrics
        };
    }
};

class WaterSurface : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit WaterSurface(const WaterSurfaceInfo& info = WaterSurfaceInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void processWaterSurface();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WaterSurface() = default;     // Default destructor
};

} // namespace hd
