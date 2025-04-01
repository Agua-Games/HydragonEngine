/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GuardPatrolAI.h
 * @brief Header file for the GuardPatrolAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - GuardPatrolAI is a class that represents a guard patrol AI in Hydragon.
 * - It is used to represent and process guard patrol AI.
 * - It uses the Vulkan API for guard patrol AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct GuardPatrolAIInfo : public NodeInfo {
    GuardPatrolAIInfo() {
        nodeType = "Gameplay/GuardPatrolAI";
        
        inputs = {
            "guardPatrolData",  // Guard patrol data
            "environment",      // Environment data
            "characterData",    // Character data
            "guardPatrolState"  // Guard patrol state
        };
        
        outputs = {
            "guardPatrolStatus", // Guard patrol status
            "guardPatrolMetrics" // Guard patrol performance metrics
        };
    }
};

class GuardPatrolAI : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit GuardPatrolAI(const GuardPatrolAIInfo& info = GuardPatrolAIInfo())
        : BehaviorTree(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processGuardPatrolAI();
    void processNode() override {
        processGuardPatrolAI(); 
    }
    void addGuard(const std::string& guardName);
    void removeGuard(const std::string& guardName);
    void updateGuardPatrol();
    void updateGuard(const std::string& guardName);
    void addPath(const std::string& pathName);
    void removePath(const std::string& pathName);
    void addPathPoint(const std::string& pathName, const std::string& pointName);
    void removePathPoint(const std::string& pathName, const std::string& pointName);
    void updatePath(const std::string& pathName);  
    void updatePathPoint(const std::string& pathName, const std::string& pointName);  
    void update() override {
        processGuardPatrolAI();
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GuardPatrolAI() = default;     // Default destructor
};

} // namespace hd
