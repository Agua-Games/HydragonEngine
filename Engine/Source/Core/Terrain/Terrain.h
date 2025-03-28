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
#include "ProceduralManager.h"
#include "Heightmap.h"

namespace hd {

struct TerrainInfo : public NodeInfo {
    TerrainInfo() {
        nodeType = "Terrain/Terrain";
        
        inputs = {
            "terrainType",      // Type of terrain (mountain, desert, etc.)
            "heightMap",        // Heightmap data
            "seed",
            "scale",
            "resolution",       // Resolution of the terrain
            "octaves",
            "persistence",
            "lacunarity",
            "subdivisionLevel", // Subdivision level for detail
            "detailLevels",     // Detail levels for procedural generation
            "noiseParams",      // Procedural noise parameters
            "deformation",      // Terrain deformation parameters
            "erosionParams",    // Erosion parameters
            "lodParams",        // Level of detail parameters
            "adaptiveQuality"   // Adaptive quality parameters
        };
        
        outputs = {
            "terrainData",      // Generated terrain data
            "heightMap",        // Heightmap data
            "normalMap",        // Normal map data
            "detailMap",        // Detail map data
            "lodData",          // Level of detail data
            "performanceMetrics"
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
    void initialize() override {}
    void load() override {}

    // Set default values
    TerrainType terrainType = TerrainType::Mountain;
    uint32_t seed = 0;
    float scale = 1.0f;
    float resolution = 1024;
    int octaves = 1;
    float persistence = 0.5f;
    float lacunarity = 2.0f;
    int subdivisionLevel = 1;
    int detailLevels = 1;
    Heightmap heightMap;
    ProceduralNoiseParams noiseParams;
    TerrainDeformation deformation;
    ErosionParams erosionParams;
    LODParams lodParams;
    AdaptiveQualityParams adaptiveQuality;
    TerrainData terrainData;

    // === Processing ===
    void processNode() override {
        terrainType = getInputValue<TerrainType>("terrainType");
        seed = getInputValue<uint32_t>("seed");
        scale = getInputValue<float>("scale");
        resolution = getInputValue<float>("resolution");
        octaves = getInputValue<int>("octaves");
        persistence = getInputValue<float>("persistence");
        lacunarity = getInputValue<float>("lacunarity");
        subdivisionLevel = getInputValue<int>("subdivisionLevel");
        detailLevels = getInputValue<int>("detailLevels");
        noiseParams = getInputValue<ProceduralNoiseParams>("noiseParams");
        deformation = getInputValue<TerrainDeformation>("deformation");
        erosionParams = getInputValue<ErosionParams>("erosionParams");
        lodParams = getInputValue<LODParams>("lodParams");
        adaptiveQuality = getInputValue<AdaptiveQualityParams>("adaptiveQuality");

        processTerrain();

        // Set outputs
        setOutputValue("terrainData", terrainData);
        setOutputValue("heightMap", heightMap);
        setOutputValue("normalMap", normalMap);
        setOutputValue("detailMap", detailMap);
        setOutputValue("lodData", lodData);
        setOutputValue("performanceMetrics", performanceMetrics);
    }
    void processTerrain();
    void generateTerrain();
    void convertToHeightmapMesh();
    void convertToAdaptiveMesh();
    void convertToVoxelMesh();
    // Bake/boolean operation from static mesh to terrain. Useful for voxelized sections.
    void performBooleanOperation(BooleanOperation::Mode mode, const std::string& meshAsset);
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Terrain() = default;
};

} // namespace hd

