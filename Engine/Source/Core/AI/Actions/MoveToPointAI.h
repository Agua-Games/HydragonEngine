/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MovetoPointAI.h
 * @brief Header file for the MovetoPointAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - MovetoPointAI is a class that represents a behavior AI action in Hydragon.
 * - It is used to represent and process AI actions.
 * - It uses the Vulkan API for action AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Node.h"
#include "DataTable.h"
#include "glm/glm.hpp"

namespace hd {

struct MovetoPointAIInfo : public NodeInfo {
    MovetoPointAIInfo() {
        nodeType = "AI/MovetoPointAI";

        inputs = {
            "targetPosition",  // Target position for movement
            "speed",           // Movement speed
            "tolerance",       // Tolerance for reaching the target position
            "waypoints",       // Waypoints for navigation
            "waypointGroup",   // Waypoint group name
            "environment",     // Environment data
            "proceduralParams" // Procedural parameters
        };
        outputs = {
            "status",          // Status of the movement
            "metrics"          // Performance metrics
        };
    }
}; 

class MovetoPointAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MovetoPointAI(const MovetoPointAIInfo& info = MovetoPointAIInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    glm::vec3 targetPosition;
    float speed;
    float tolerance;
    std::vector<glm::vec3> waypoints;
    std::string waypointGroup;
    DataTable environment;
    DataTable proceduralParams;

    // === Processing ===   
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MovetoPointAI() = default;     // Default destructor
};

} // namespace hd
