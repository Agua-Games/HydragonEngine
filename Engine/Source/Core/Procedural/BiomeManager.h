/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BiomeManager.h
 * @brief Header file for the BiomeManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - BiomeManager is a class that represents a biome manager node in the engine's node graph.
 * - It is responsible for managing biomes in Hydragon.
 * - It supports procedural features, such as terrain generation, vegetation generation, and weather generation. Also supports two-way messaging with other biomes, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"
#include "Biome.h"

namespace hd {
struct BiomeManagerInfo : public NodeInfo {
    BiomeManagerInfo() {
        nodeType = "Procedural/BiomeManager";
        
        inputs = {
            "biomeData",        // Biome data
            "environment",      // Environment data
            "characterData",    // Character data
            "biomeState"        // Biome state
        };
        
        outputs = {
            "biomeStatus",      // Biome status
            "biomeMetrics"      // Biome performance metrics
        };
    }
};

class BiomeManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BiomeManager(const BiomeManagerInfo& info = BiomeManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BiomeManager() = default;     // Default destructor
};

} // namespace hd