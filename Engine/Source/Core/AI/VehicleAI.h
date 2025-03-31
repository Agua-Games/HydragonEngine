/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VehicleAI.h
 * @brief Header file for the VehicleAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - VehicleAI is a class that represents a vehicle AI in Hydragon.
 * - It is used to represent and process vehicle AI.
 * - It uses the Vulkan API for vehicle AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "Node.h"
#include "BehaviorTree.h"
#include "Vehicle.h"
#include "AgentAI.h"
#include "AIManager.h"
#include "Waypoint.h"
#include "DataTable.h"

namespace hd {

enum class VehicleAIStrategy {
    Passive,
    Defensive,
    Aggressive
};

struct VehicleAIInfo : public BehaviorTreeInfo {
    VehicleAIInfo() {
        nodeType = "AI/VehicleAI";
        
        inputs = {
            "vehicle",           // Vehicle data
            "behaviorTree",      // Behavior tree data
            "environment",       // Environment data
            "proceduralParams"   // Procedural parameters
        };
        
        outputs = {
            "vehicleState",      // Vehicle state
            "behaviorMetrics",   // Behavior tree performance metrics
            "proceduralData"     // Generated procedural data
        };
    }
};

class VehicleAI : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit VehicleAI(const VehicleAIInfo& info = VehicleAIInfo())
        : BehaviorTree(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    VehicleAIStrategy strategy = VehicleAIStrategy::Passive;
    float supportRange = 10.0f;
    float attackRange = 20.0f;
    float retreatRange = 30.0f;
    float speed = 10.0f;
    float tolerance = 0.1f;
    float cooldown = 1.0f;
    float duration = 5.0f;
    std::vector<Waypoint> waypoints;

    // === Processing ===
    void processVehicleAI();
    void processNode() override {
        processVehicleAI(); 
    }    
    void update() override {
        processVehicleAI(); 
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~VehicleAI() = default;     // Default destructor
};

} // namespace hd