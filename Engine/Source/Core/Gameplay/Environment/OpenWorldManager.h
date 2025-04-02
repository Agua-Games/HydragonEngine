/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file OpenWorldManager.h
 * @brief Header file for the OpenWorldManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - OpenWorldManager is a singleton class that manages the open world in the game.
 * - It is responsible for managing the open world in the game.
 * - It supports interactive features, such as exploring, discovering, and interacting with the open world. Also supports two-way messaging with other open world, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "Scene.h"

namespace hd {
struct OpenWorldManagerInfo : public NodeInfo {
    OpenWorldManagerInfo() {
        nodeType = "Gameplay/OpenWorldManager";
        
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

class OpenWorldManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit OpenWorldManager(const OpenWorldManagerInfo& info = OpenWorldManagerInfo())
        : Node(info) {} // Default constructor for OpenWorldManager class
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~OpenWorldManager() = default;     // Default destructor
};

} // namespace hd
