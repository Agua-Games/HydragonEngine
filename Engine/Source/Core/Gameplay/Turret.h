/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Turret.h
 * @brief Header file for the Turret class.
 * 
 * ARCHITECTURAL NOTES:
 * - Turret is a class that represents a turret in Hydragon.
 * - It is used to represent any turret in the game world, such as machine guns, cannons, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other turrets, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "SentryWeapon.h"
#include "Equipment.h"

namespace hd {

struct TurretInfo : public SentryWeaponInfo {
    TurretInfo() {
        nodeType = "Gameplay/Turret";
        
        inputs = {
            "turretType",        // Type of turret (machine gun, cannon, etc.)
            "turretData",        // Turret data
            "environment",       // Environment data
            "characterData",     // Character data
            "turretState"        // Turret state
        };
        
        outputs = {
            "turretStatus",      // Turret status
            "turretMetrics"      // Turret performance metrics
        };
    }
};

class Turret : public SentryWeapon, public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit Turret(const TurretInfo& info = TurretInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void detectTargets();
    void chooseTarget();
    void fire();
    void reload();
    void aim();
    void retract(bool retract);
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Turret() = default;     // Default destructor
};

} // namespace hd
