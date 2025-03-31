/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TerrainLayer.h
 * @brief Header file for the TerrainLayer class.
 * 
 * ARCHITECTURAL NOTES:
 * - TerrainLayer is a class that represents a terrain layer node in the engine's node graph.
 * - It is used to represent and process terrain layers, each layer having its own material and other properties, functionality, etc.
 * - It fully supports procedural generation, deformation, and detailing.
 * - It uses the Vulkan API for terrain layer processing.
 */
#pragma once
#include "Node.h"
#include "Terrain.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include "PhysicsMaterial.h"

namespace hd {

struct TerrainLayerInfo : public NodeInfo {
    TerrainLayerInfo() {
        NodeType = "Terrain/TerrainLayer";
        inputs = {
            "Terrain",  // Terrain node
            "LayerData" // Layer data
        };
        outputs = {
            "LayerData" // Layer data
        };
    }
};

class TerrainLayer : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit TerrainLayer(const TerrainLayerInfo& info = TerrainLayerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    Terrain* terrain = nullptr;
    TerrainLayerData layerData;
    PhysicsMaterial material;
    float density = 0.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TerrainLayer() = default;     // Default destructor
};

} // namespace hd