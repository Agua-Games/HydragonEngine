/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TerrainManager.h
 * @brief Header file for the TerrainManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - TerrainManager is a class that represents a terrain manager node in the engine's node graph.
 * - It is responsible for managing terrains in Hydragon.
 * - It supports interactive features, such as terrain deformation and erosion.
 * - It also supports various generation and deformation techniques.
 * - It supports back and forth conversion to/from heightmaps to voxels, and replacing specific sections of the terrain with voxelized meshes - for caves, complex
 * rock formations, arches, etc. Also baking/boolean operations from static meshes to the terrain, specially useful with voxelized sections.
 * - It also supports the same AdaptiveMesh strategies for subdivision, LOD and quality adaptation.
 * - It has strong procedural generation, deformation, and detailing capabilities.
 * - It leverages adaptive quality strategies to optimize performance.
 * - It uses the Vulkan API for terrain processing.
 */
#pragma once
#include "Terrain.h"

namespace hd {

struct TerrainManagerInfo : public NodeInfo {
    TerrainManagerInfo() {
        NodeType = "Terrain/TerrainManager";
        inputs = {
            "Terrains" // Array of terrains
        };
        outputs = {
            "ActiveTerrains" // Active terrains
        };
    }
};

class TerrainManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit TerrainManager(const TerrainManagerInfo& info = TerrainManagerInfo())
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
    ~TerrainManager() = default;     // Default destructor
};

} // namespace hd
