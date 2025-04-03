/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ActionAI.h
 * @brief Header file for the ActionAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - ActionAI is a class that represents a behavior AI action in Hydragon.
 * - It is used to represent and process AI actions.
 * - It uses the Vulkan API for action AI processing.
 * 
 * @todo Change all input and output names to lowercase camelCase.
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Node.h"
#include "Math/Vector3.h"

namespace hd {

struct Waypoint {
    Vector3 position;
    float time;
};

struct ActionAIInfo : public NodeInfo {
    ActionAIInfo() {
        nodeType = "AI/ActionAI";
        
        inputs = {
            "action",               // Action data
            "environment",          // Environment data
            "proceduralParams",     // Procedural parameters
            "waypoints",            // Waypoints for navigation
            "waypointGroup",        // Waypoint group name
            "speed",                // Agent speed for movement actions
            "duration",             // Duration for actions like wait
            "minRange",             // Minimum range for actions like moveToRange, attackRange, etc.
            "preferredRange",       // Preferred range for actions like moveToRange
            "tolerance",            // Tolerance for actions like moveToRange, etc.
            "cooldown",             // Cooldown for actions like attack, etc.
        };
        
        outputs = {
            "actionResult",    // Result of the action
            "behaviorMetrics", // Performance and quality metrics
            "proceduralData"   // Generated procedural data
        };
    }
};

class ActionAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ActionAI(const ActionAIInfo& info = ActionAIInfo())
        : Node(info) {}  
        
    float preferredRange;            // Preferred range for actions like moveToRange
    float minRange;                  // Minimum range for actions like moveToRange, attackRange, etc.
    float cooldown;                  // Cooldown for actions like attack, etc.
    float speed;                     // Agent speed for movement actions
    float tolerance;                 // Tolerance for actions like moveToRange
    float duration;                  // Duration for actions like wait
    std::vector<Waypoint> waypoints;
    std::string waypointGroup;

    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    // Waypoints, Navigation, and Pathfinding
    void setWaypoints(const std::vector<Waypoint>& waypoints) {}         // Set waypoints directly from a vector of Waypoints (Vector3s and times)
    void setWaypointsFromGroup(const std::string& groupName) {}          // Set waypoints from a waypoint group

    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ActionAI() = default;     // Default destructor
};

} // namespace hd
