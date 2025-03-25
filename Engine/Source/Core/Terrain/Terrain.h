/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Terrain.h
 * @brief Header file for the Terrain class.
 * 
 * ARCHITECTURAL NOTES:
 * - Terrain is a class that represents a terrain in Hydragon.
 * - It is used to represent any terrain in the game world, such as landscapes, terrains, etc.
 * - It supports interactive features, such as terrain deformation and erosion.
 * - It has strong procedural generation, deformation, and detailing capabilities.
 * - It leverages adaptive quality strategies to optimize performance.
 * - It uses the Vulkan API for terrain processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "Noise.h"

namespace hd {

struct TerrainInfo : public NodeInfo {
    TerrainInfo() {
        NodeType = "Terrain/Terrain";
        
        inputs = {
            "TerrainType",      // Type of terrain (mountain, desert, etc.)
            "Seed",
            "Scale",
            "Octaves",
            "Persistence",
            "Lacunarity",
            "DetailLevels",
            "NoiseParams",      // Procedural noise parameters
            "Deformation",      // Terrain deformation parameters
            "ErosionParams",    // Erosion parameters
            "LODParams",        // Level of detail parameters
            "AdaptiveQuality"   // Adaptive quality parameters
        };
        
        outputs = {
            "TerrainData",      // Generated terrain data
            "HeightMap",        // Heightmap data
            "NormalMap",        // Normal map data
            "DetailMap",        // Detail map data
            "LODData",          // Level of detail data
            "PerformanceMetrics"
        };
    }
};

class Terrain : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Terrain(const TerrainInfo& info = TerrainInfo())
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
    ~Terrain() = default;
};

} // namespace hd

