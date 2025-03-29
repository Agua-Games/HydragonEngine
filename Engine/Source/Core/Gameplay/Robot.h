/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Robot.h
 * @brief Header file for the Robot class.
 * 
 * ARCHITECTURAL NOTES:
 * - Robot is a class that represents a robot in Hydragon.
 * - It is used to represent any robot in the game world, such as drones, androids, etc.
 * - It supports interactive features, such as moving, attacking, and defending. Also supports two-way messaging with other robots, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct RobotInfo : public CharacterInfo {
    RobotInfo() {
        nodeType = "Gameplay/Robot";
        
        inputs = {
            "robotType",        // Type of robot (drone, android, etc.)
            "robotData",        // Robot data
            "environment",      // Environment data
            "characterData",    // Character data
            "robotState"        // Robot state
        };
        
        outputs = {
            "robotStatus",      // Robot status
            "robotMetrics"      // Robot performance metrics
        };
    }
};

class Robot : public Character {
public:
    // === Allocation, Initialization, Loading ===
    explicit Robot(const RobotInfo& info = RobotInfo())
        : Character(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void move();
    void attack();
    void defend();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Robot() = default;     // Default destructor
} // namespace hd
