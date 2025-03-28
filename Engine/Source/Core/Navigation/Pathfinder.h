/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Pathfinder.h
 * @brief Header file for the Pathfinder class.
 * 
 * ARCHITECTURAL NOTES:
 * - Pathfinder is a class that represents a pathfinder in Hydragon.
 * - It is used to represent and process pathfinding, for characters, vehicles, probes, etc. (AI).
 * - It uses the Vulkan API for pathfinding processing (Compute).
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {
    struct PathfinderInfo : public NodeInfo {
        PathfinderInfo() {
            nodeType = "Navigation/Pathfinder";
            
            inputs = {
                "pathData",        // Path data
                "pathType",        // Type of path
                "pathParams"       // Path parameters
            };
            
            outputs = {
                "pathStatus",      // Path status
                "pathMetrics"      // Path performance metrics
            };
        }
    };

    class Pathfinder : public Node {
    public:
        // === Allocation, Initialization, Loading === 
        explicit Pathfinder(const PathfinderInfo& info = PathfinderInfo())
            : Node(info) {}
        initialize() override {}
        load() override {}

        // === Processing ===
        void findPath();
        void navigatePath();
        void processPath();
        void processNode() override { }
        void update();

        // === Cleanup ===
        void unload() override {}
        void cleanup() override {}
        ~Pathfinder() = default;     // Default destructor
    };

} // namespace hd

