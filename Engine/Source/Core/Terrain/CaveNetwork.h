/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CaveNetwork.h
 * @brief Header file for the CaveNetwork class.
 * 
 * ARCHITECTURAL NOTES:
 * - CaveNetwork is a class that represents a cave network node in the engine's node graph.
 * - It is responsible for generating and managing cave networks in Hydragon.
 * - It supports various generation and deformation techniques, such as erosion, deposition, and cave formation. And cave features like stalactites, stalagmites,
 * echoes, crumbling rocks and events, etc.
 * - It supports back and forth conversion to/from heightmaps to voxels, and replacing specific sections of the terrain with voxelized meshes - for caves, complex
 * rock formations, arches, etc. Also baking/boolean operations from static meshes to the terrain, specially useful with voxelized sections.
 * - It uses the Vulkan API for cave network processing.
 */
#pragma once
#include "Terrain.h"
#include "ProceduralManager.h"

namespace hd {

struct CaveNetworkInfo : public TerrainInfo {
    CaveNetworkInfo() {
        nodeType = "Procedural/CaveNetwork";
        
        inputs = {
            "noiseType",      // Perlin, Simplex, Worley, etc.
            "seed",
            "scale",
            "resolution",     // Resolution of the heightmap (width and height)
            "octaves",
            "persistence",
            "lacunarity",
            "domain",         // 2D or 3D
            "warp"           // Optional domain warping
        };
        
        outputs = {
            "heightmapValue",
            "derivatives",    // For normal map generation
            "heightmapMetrics" // Statistical properties
        };
    }
};

class CaveNetwork : public Terrain {
public:
    // === Allocation, Initialization, Loading ===
    explicit CaveNetwork(const CaveNetworkInfo& info = CaveNetworkInfo())
        : Terrain(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // Set default values
    NoiseType noiseType = NoiseType::Perlin;
    float scale = 1.0f;
    float resolution = 1024;
    bool generatePaths = false;

    // === Processing ===
    void processNode() override {}    // Override the base class processNode() method
    void generatePaths();
    void generateCaveNetwork();    // Generate cave network data and store it in the heightMap member variable.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CaveNetwork() = default;     // Default destructor
};

} // namespace hd
