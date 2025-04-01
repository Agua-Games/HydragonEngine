/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Powerup.h
 * @brief Header file for the Powerup class.
 * 
 * ARCHITECTURAL NOTES:
 * - Powerup is a class that represents a powerup in Hydragon.
 * - It is used to represent any powerup in the game world, such as health packs, ammo crates, etc.
 * - It supports interactive features, such as picking up, using, and dropping. Also supports two-way messaging with other powerups, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

enum class PowerupType {
    health,
    ammo,
    weapon,
    armor,
    custom
};

struct PowerupInfo : public NodeInfo {
    PowerupInfo() {
        nodeType = "Gameplay/Powerup";
        
        inputs = {
            "powerupType",     // Type of powerup (health, ammo, etc.)
            "powerupData",     // Powerup data
            "environment",     // Environment data
            "characterData",   // Character data
            "powerupState"     // Powerup state
        };
        
        outputs = {
            "powerupStatus",   // Powerup status
            "powerupMetrics"   // Powerup performance metrics
        };
    }
};

class Powerup : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Powerup(const PowerupInfo& info = PowerupInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    PowerupType type = PowerupType::custom;
    float duration = 0.0f;
    float spawnInterval = 0.0f;
    float range = 0.0f;
    float radius = 0.0f;
    float cooldown = 0.0f;
    DataTable powerupData;

    // === Processing ===
    void processNode() override { };     // Process the node, based on the input values.
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Powerup() = default;     // Default destructor
};

} // namespace hd
