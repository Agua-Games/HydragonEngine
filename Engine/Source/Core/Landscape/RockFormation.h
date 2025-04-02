/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RockFormation.h
 * @brief Header file for the RockFormation class.
 * 
 * ARCHITECTURAL NOTES:
 * - RockFormation is a class that represents a rock formation in Hydragon.
 * - It is used to represent any rock formation in the game world, such as boulders, cliffs, etc.
 * - It supports procedural features, such as rock generation, rock growth, and rock "death" (cracks, debris, crumbling). Also supports two-way messaging with other rock formations, environment, character, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other rock formations, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralManager.h"
#include "Noise.h"
#include "Terrain.h"
#include "Heightmap.h"

namespace hd {

struct RockFormationInfo : public NodeInfo {
    RockFormationInfo() {
        nodeType = "Procedural/RockFormation";
        
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

class RockFormation : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit RockFormation(const RockFormationInfo& info = RockFormationInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void generateRockFormation();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RockFormation() = default;     // Default destructor
};

} // namespace hd
