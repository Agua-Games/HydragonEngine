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
        NodeType = "AI/ActionAI";
        
        inputs = {
            "Action",               // Action data
            "Environment",          // Environment data
            "ProceduralParams",     // Procedural parameters
            "Waypoints",            // Waypoints for navigation
            "WaypointGroup",        // Waypoint group name
            "Speed",                // Agent speed for movement actions
            "Duration",             // Duration for actions like wait
            "MinRange",             // Minimum range for actions like moveToRange, attackRange, etc.
            "PreferredRange",       // Preferred range for actions like moveToRange
            "Tolerance",            // Tolerance for actions like moveToRange, etc.
            "Cooldown",             // Cooldown for actions like attack, etc.
        };
        
        outputs = {
            "ActionResult",    // Result of the action
            "BehaviorMetrics", // Performance and quality metrics
            "ProceduralData"   // Generated procedural data
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

    // === Processing ===
    // Waypoints, Navigation, and Pathfinding
    void setWaypoints(const std::vector<Waypoint>& waypoints) {}         // Set waypoints directly from a vector of Waypoints (Vector3s and times)
    void setWaypointsFromGroup(const std::string& groupName) {}          // Set waypoints from a waypoint group

    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ActionAI() = default;     // Default destructor
};

} // namespace hd
