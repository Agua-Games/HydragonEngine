/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NavigationManager.h
 * @brief Header file for the NavigationManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - NavigationManager is a class that represents navigation and is responsible for managing navigation in Hydragon.
 * - It uses the Vulkan API (Compute) along with CPU for navigation processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {
    struct NavigationManagerInfo : public NodeInfo {
        NavigationManagerInfo() {
            nodeType = "Navigation/NavigationManager";
            
            inputs = {
                "navigationData",  // Navigation data
                "environment",     // Environment data
                "characterData",   // Character data
                "navigationState"  // Navigation state
            };
            
            outputs = {
                "navigationStatus", // Navigation status
                "navigationMetrics" // Navigation performance metrics
            };
        }
    };

    class NavigationManager : public Node {
    public:
        // === Allocation, Initialization, Loading === 
        explicit NavigationManager(const NavigationManagerInfo& info = NavigationManagerInfo())
            : Node(info) {}
        initialize() override {}
        load() override {}

        // === Processing ===
        void processNode() override { }
        void update();

        // === Cleanup ===
        void unload() override {}
        void cleanup() override {}
        ~NavigationManager() = default;     // Default destructor
    };

} // namespace hd

