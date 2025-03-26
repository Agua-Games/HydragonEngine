/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Navmesh.h
 * @brief Header file for the Navmesh class.
 * 
 * ARCHITECTURAL NOTES:
 * - Navmesh is a class that represents a navigation mesh in Hydragon.
 * - It is used to represent and process navigation meshes.
 * - It uses the Vulkan API for navigation mesh processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include "Node.h"

namespace hd {

struct NavmeshInfo : public NodeInfo {
    NavmeshInfo() {
        nodeType = "Navigation/Navmesh";
        
        inputs = {
            "meshData",        // Mesh data
            "navmeshSettings", // Navigation mesh settings
            "navmeshType",     // Type of navigation mesh
            "navmeshParams"    // Navigation mesh parameters
        };
        
        outputs = {
            "navmeshData",     // Navigation mesh data
            "navmeshMetrics"   // Navigation mesh performance metrics
        };
    }
};

class Navmesh : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Navmesh(const NavmeshInfo& info = NavmeshInfo())
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
    ~Navmesh() = default;     // Default destructor
};

} // namespace hd

