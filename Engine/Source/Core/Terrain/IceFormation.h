/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file IceFormation.h
 * @brief Header file for the IceFormation class.
 * 
 * ARCHITECTURAL NOTES:
 * - IceFormation is a class that represents an ice formation node in the engine's node graph.
 * - It is used to generate and manage ice formations for terrain generation.
 * 
 */
#pragma once
#include "Terrain.h"
#include "ProceduralTypes.h"
#include "ProceduralManager.h"
#include "Noise.h"
#include "Heightmap.h"

namespace hd {

struct IceFormationInfo : public TerrainInfo {
    IceFormationInfo() {
        nodeType = "Procedural/IceFormation";
        
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

class IceFormation : public Terrain {
public:
    // === Allocation, Initialization, Loading ===
    explicit IceFormation(const IceFormationInfo& info = IceFormationInfo())
        : Terrain(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // Set default values
    NoiseType noiseType = NoiseType::Perlin;
    float scale = 1.0f;
    float resolution = 1024;
    float thickness = 1.0f;
    float slipperiness = 0.5f;

    // === Processing ===
    void processNode() override {}    // Override the base class processNode() method
    void generateIceFormation();    // Generate ice formation data and store it in the heightMap member variable.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~IceFormation() = default;     // Default destructor
};

} // namespace hd
