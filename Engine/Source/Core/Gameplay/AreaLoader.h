/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AreaLoader.h
 * @brief Header file for the AreaLoader class.
 * 
 * ARCHITECTURAL NOTES:
 * - AreaLoader is a class that represents an area loader in Hydragon.
 * - It is used to load and unload areas in the game world.
 * - It supports loading and unloading of areas, as well as managing the loading and unloading of assets.
 */
#pragma once
#include "Node.h"

namespace hd {

struct AreaLoaderInfo : public NodeInfo {
    AreaLoaderInfo() {
        nodeType = "Gameplay/AreaLoader";
        
        inputs = {
            "areaData",        // Area data
            "areaState",       // Area state
            "assetData",       // Asset data
            "assetState",      // Asset state
            "loadingParams",   // Loading parameters
            "unloadingParams"  // Unloading parameters
        };
        
        outputs = {
            "areaStatus",      // Area status
            "assetStatus",     // Asset status
            "loadingMetrics",  // Loading performance metrics
            "unloadingMetrics" // Unloading performance metrics
        };
    }
};

class AreaLoader : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AreaLoader(const AreaLoaderInfo& info = AreaLoaderInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AreaLoader() = default;     // Default destructor
};

} // namespace hd