/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FortressWall.h
 * @brief Header file for the FortressWall class.
 * 
 * ARCHITECTURAL NOTES:
 * - FortressWall is a class that represents a fortress wall in Hydragon.
 * - It is used to represent any fortress wall in the game world, such as castle walls, fort walls, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other fortress walls, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Building.h"

namespace hd {

struct FortressWallInfo : public BuildingInfo {
    FortressWallInfo() {
        nodeType = "Gameplay/FortressWall";
        
        inputs = {
            "model",        // Model of the fortress wall
            "position",     // Position of the fortress wall
            "rotation",     // Rotation of the fortress wall
            "scale",        // Scale of the fortress wall
            "material",     // Material of the fortress wall
            "animation",    // Animation of the fortress wall
            "physics",      // Physics of the fortress wall
            "collision",    // Collision of the fortress wall
            "script"        // Script of the fortress wall
        };
        
        outputs = {
            "status",       // Status of the fortress wall
            "metrics"       // Performance metrics of the fortress wall
        };
    }
};

class FortressWall : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit FortressWall(const FortressWallInfo& info = FortressWallInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void enter();
    void exit();
    void interact();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FortressWall() = default;     // Default destructor
};

} // namespace hd