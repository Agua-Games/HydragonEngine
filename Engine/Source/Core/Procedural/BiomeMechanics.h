/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BiomeMechanics.h
 * @brief Header file for the BiomeMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - BiomeMechanics is a class that represents biome mechanics in Hydragon.
 * - It is used to represent any biome mechanics in the game world, such as biome generation, biome management, biome communication, biome objectives, etc.
 * - It supports procedural features, such as biome generation, biome management, biome communication, biome objectives, etc. Also supports two-way messaging with other biome mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"
#include "Biome.h"

namespace hd {
struct BiomeMechanicsInfo : public NodeInfo {
    BiomeMechanicsInfo() {
        nodeType = "Procedural/BiomeMechanics";
        
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

class BiomeMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BiomeMechanics(const BiomeMechanicsInfo& info = BiomeMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BiomeMechanics() = default;     // Default destructor
};

} // namespace hd