/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GameplayArea.h
 * @brief Header file for the GameplayArea class.
 * 
 * ARCHITECTURAL NOTES:
 * - GameplayArea is a class that represents a gameplay area in Hydragon.
 * - It is used to represent any gameplay area in the game world, such as battlefields, arenas, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other gameplay areas, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
//#include "glm/gtx/vector_angle.hpp" // For glm::angle() function
#include "Node.h"
#include "DataTable.h"
#include "Ability.h"
#include "PlayerSpawner.h"

namespace hd {

enum class GameplayAreaType {
    Battlefield,
    Arena,
    Platforms,          // For platform games
    Custom
};

struct GameplayAreaInfo : public NodeInfo {
    GameplayAreaInfo() {
        nodeType = "Gameplay/GameplayArea";
        
        inputs = {
            "areaType",        // Type of gameplay area (battlefield, arena, etc.)
            "areaData",        // Gameplay area data
            "environment",     // Environment data
            "characterData",   // Character data
            "areaState"        // Gameplay area state
        };
        
        outputs = {
            "areaStatus",      // Gameplay area status
            "areaMetrics"      // Gameplay area performance metrics
        };
    }
};

class GameplayArea : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit GameplayArea(const GameplayAreaInfo& info = GameplayAreaInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    GameplayAreaType type = GameplayAreaType::Custom;
    std::vector<PlayerSpawner> respawnPoints;

    // === Processing ===
    void processNode() override {
 
    }
    void setRespawnPoint();
    void enter();
    void exit();
    void interact();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GameplayArea() = default;     // Default destructor
};

} // namespace hd