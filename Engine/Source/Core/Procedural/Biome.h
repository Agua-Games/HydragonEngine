/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Biome.h
 * @brief Header file for the Biome class.
 * 
 * ARCHITECTURAL NOTES:
 * - Biome is a class that represents a biome in Hydragon.
 * - It is used to represent any biome in the game world, such as forest, desert, etc.
 * - It supports procedural features, such as terrain generation, vegetation generation, and weather generation. Also supports two-way messaging with other biomes, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"
#include "Terrain.h"
#include "Vegetation.h"
#include "Weather.h"

namespace hd {
struct BiomeInfo : public NodeInfo {
    BiomeInfo() {
        nodeType = "Procedural/Biome";
        
        inputs = {
            "biomeType",        // Type of biome (forest, desert, etc.)
            "terrain",          // Terrain data
            "vegetation",       // Vegetation data
            "weather",          // Weather data
            "biomeState"        // Biome state
        };
        
        outputs = {
            "biomeStatus",      // Biome status
            "biomeMetrics"      // Biome performance metrics
        };
    }
};

class Biome : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Biome(const BiomeInfo& info = BiomeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float transitionWidth = 0.0f;
    bool dynamicBlending = false;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Biome() = default;     // Default destructor
};

} // namespace hd