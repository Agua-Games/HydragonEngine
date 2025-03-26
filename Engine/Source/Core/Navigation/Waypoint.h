/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Waypoint.h
 * @brief Header file for the Waypoint class.
 * 
 * ARCHITECTURAL NOTES:
 * - Waypoint is a class that represents a waypoint in Hydragon.
 * - It is used to represent and process waypoints.
 * - It uses the Vulkan API for waypoint processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include "Node.h"
    
namespace hd {

struct WaypointInfo : public NodeInfo {
    WaypointInfo() {
        nodeType = "Navigation/Waypoint";
        
        inputs = {
            "waypointData",    // Waypoint data
            "waypointType",    // Type of waypoint
            "waypointState"    // Waypoint state
        };
        
        outputs = {
            "waypointStatus",  // Waypoint status
            "waypointMetrics"  // Waypoint performance metrics
        };
    }
};

class Waypoint : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Waypoint(const WaypointInfo& info = WaypointInfo())
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
    ~Waypoint() = default;     // Default destructor
};

} // namespace hd

