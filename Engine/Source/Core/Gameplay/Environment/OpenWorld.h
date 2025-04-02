/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file OpenWorld.h
 * @brief Header file for the OpenWorld class.
 * 
 * ARCHITECTURAL NOTES:
 * - OpenWorld is a class that represents an open world in Hydragon.
 * - It is used to represent any open world in the game world, such as wilderness, etc.
 * - It supports interactive features, such as exploring, discovering, and interacting with the open world. Also supports two-way messaging with other open world, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "Scene.h"
#include "DataTable.h"

namespace hd {
struct OpenWorldInfo : public NodeInfo {
    OpenWorldInfo() {
        nodeType = "Gameplay/OpenWorld";
        
        inputs = {
            "openWorldData",    // Open world data
            "environment",      // Environment data
            "characterData",    // Character data
            "openWorldState"    // Open world state
        };
        
        outputs = {
            "openWorldStatus",  // Open world status
            "openWorldMetrics"  // Open world performance metrics
        };
    }
};

class OpenWorld : public Node {
public:
    // === Structure Definitions ===
    enum class loadingMode {
        streaming,
        progressive,
        dynamic,
        other
    };

    enum class loadingPrioritization {
        distance,
        viewDirection,
        questRelevance,
        other
    };

    enum class loadingPriority {
        high,
        medium,
        low,
        other
    };

    struct OpenWorldData {
        // Define the data structure for open world data
    };

    // === Allocation, Initialization, Loading ===
    explicit OpenWorld(const OpenWorldInfo& info = OpenWorldInfo())
        : Node(info) {} // Default constructor for OpenWorld class
    initialize() override {}
    load() override {}
    
    // Set default values
    DataTable openWorldData;
    DataTable openWorldState;
    int lodLevels = 0;
    bool infiniteHorizon = false;
    int streamingBudget = 0;
    int streamingPriority = 0;
    int streamingChunkSize = 0;
    int streamingLoadDistance = 0;
    int streamingUnloadDistance = 0;
    bool streamingAsyncLoading = false;
    Scene scene;

    // === Processing ===
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~OpenWorld() = default;     // Default destructor
};

} // namespace hd
