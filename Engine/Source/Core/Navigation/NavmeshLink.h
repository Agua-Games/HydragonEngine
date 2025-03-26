/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NavmeshLink.h
 * @brief Header file for the NavmeshLink class.
 * 
 * ARCHITECTURAL NOTES:
 * - NavmeshLink is a class that represents a navigation mesh link in Hydragon.
 * - It is used to represent and process navigation mesh links.
 * - It uses the Vulkan API for navigation mesh link processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include "Node.h"

namespace hd {

struct NavmeshLinkInfo : public NodeInfo {
    NavmeshLinkInfo() {
        nodeType = "Navigation/NavmeshLink";
        
        inputs = {
            "navmeshData",     // Navigation mesh data
            "linkData",        // Link data
            "linkType",        // Type of link
            "linkParams"       // Link parameters
        };
        
        outputs = {
            "linkStatus",      // Link status
            "linkMetrics"      // Link performance metrics
        };
    }
};

class NavmeshLink : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit NavmeshLink(const NavmeshLinkInfo& info = NavmeshLinkInfo())
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
    ~NavmeshLink() = default;     // Default destructor
};

} // namespace hd
