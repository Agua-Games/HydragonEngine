/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Path.h
 * @brief Header file for the Path class.
 * 
 * ARCHITECTURAL NOTES:
 * - Path is a class that represents a path in Hydragon.
 * - It is used to represent and process paths.
 * - It uses the Vulkan API for path processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include "Node.h"

namespace hd {
    struct PathInfo : public CurveInfo {
        PathInfo() {
            nodeType = "Navigation/Path";
            
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

    class Path : public Node {
    public:
        // === Allocation, Initialization, Loading === 
        explicit Path(const PathInfo& info = PathInfo())
            : Node(info) {}
        initialize() override {}
        load() override {}

        // === Processing ===
        void processNode() override { }
        void update();

        // === Cleanup ===
        void unload() override {}
        void cleanup() override {}
        ~Path() = default;     // Default destructor
    };

} // namespace hd

