/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NavigationMechanics.h
 * @brief Header file for the NavigationMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - NavigationMechanics is a class that represents navigation mechanics in Hydragon.
 * - It is used to represent any navigation mechanics in the game world, such as pathfinding, navigation, etc.
 * - It supports interactive features, such as pathfinding, navigation, and movement. Also supports two-way messaging with other navigation mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {
    struct NavigationMechanicsInfo : public NodeInfo {
        NavigationMechanicsInfo() {
            nodeType = "Navigation/NavigationMechanics";
            
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

    class NavigationMechanics : public Node {
    public:
        // === Allocation, Initialization, Loading === 
        explicit NavigationMechanics(const NavigationMechanicsInfo& info = NavigationMechanicsInfo())
            : Node(info) {}
        initialize() override {}
        load() override {}

        // Set default values
        DataTable navigationMechanicsData;
        bool pathfinding = false;
        bool obstacleAvoidance = false;
        float speed = 0.0f;
        float acceleration = 0.0f;
        float deceleration = 0.0f;
        float steering = 0.0f;
        DataTable navigationMechanicsState;

        // === Processing ===
        void processNode() override { }
        void update();

        // === Cleanup ===
        void unload() override {}
        void cleanup() override {}
        ~NavigationMechanics() = default;     // Default destructor
    };

} // namespace hd

