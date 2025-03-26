/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Terrain.h
 * @brief Terrain represents a terrain node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Terrain nodes are used to represent terrains in the game world.
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

/**
 * @class Terrain
 * @brief Represents a terrain node in the engine's node graph.
 * It has strong procedural generation, deformation, and detailing capabilities.
 */
class Terrain : public Node {
public:
    // === Structure Definitions ===
    struct BooleanOperation {
        enum class Mode {
            Union,
            Difference,
            Intersection,
            Slice
        };
    };

    // === Allocation, Initialization, Loading ===
    explicit Terrain(const TerrainInfo& info = TerrainInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void setNoiseParams(const NoiseParams& params);
    void setDetailLevels(int levels);
    void setDeformationParams(const DeformationParams& params);
    void setErosionParams(const ErosionParams& params);
    void setLODParams(const LODParams& params);
    void setAdaptiveQualityParams(const AdaptiveQualityParams& params);
    void convertToHeightmapMesh();
    void convertToAdaptiveMesh();
    void convertToVoxelMesh();
    // Bake/boolean operation from static mesh to terrain. Useful for voxelized sections.
    void performBooleanOperation(BooleanOperation::Mode mode, const std::string& meshAsset);
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Terrain() = default;
};

} // namespace hd

