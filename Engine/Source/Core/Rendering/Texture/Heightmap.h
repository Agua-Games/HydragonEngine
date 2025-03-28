/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Heightmap.h
 * @brief Header file for the Heightmap class.
 * 
 * ARCHITECTURAL NOTES:
 * - Heightmap is a class that represents a heightmap node in the engine's node graph.
 * - It is used to generate and manage heightmaps for terrain generation.
 * 
 */
#pragma once
#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "Noise.h"

namespace hd {

struct HeightmapInfo : public NodeInfo {
    HeightmapInfo() {
        NodeType = "Procedural/Heightmap";
        
        inputs = {
            "NoiseType",      // Perlin, Simplex, Worley, etc.
            "Seed",
            "Scale",
            "Resolution",     // Resolution of the heightmap (width and height)
            "Octaves",
            "Persistence",
            "Lacunarity",
            "Domain",         // 2D or 3D
            "Warp"           // Optional domain warping
        };
        
        outputs = {
            "HeightmapValue",
            "Derivatives",    // For normal map generation
            "HeightmapMetrics" // Statistical properties
        };
    }
};

class Heightmap : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Heightmap(const HeightmapInfo& info = HeightmapInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    NoiseType noiseType = NoiseType::Perlin;
    float scale = 1.0f;
    float resolution = 1024;

    // === Processing ===
    void processNode() override {
 
    }
    void generateHeightmap();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Heightmap() = default;     // Default destructor
};

} // namespace hd
