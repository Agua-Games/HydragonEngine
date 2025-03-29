/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MountedCombat.h
 * @brief Header file for the MountedCombat class.
 * 
 * ARCHITECTURAL NOTES:
 * - MountedCombat is a class that represents mounted combat mechanics in Hydragon.
 * - It is used to represent any mounted combat mechanics in the game world, such as cavalry, mounted archery, etc.
 * - It supports interactive features, such as charging, firing, and dodging. Also supports two-way messaging with other mounted combat, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Math.h"

namespace hd {

struct MountedCombatInfo : public NodeInfo {
    MountedCombatInfo() {
        nodeType = "Gameplay/MountedCombat";
        
        inputs = {
            "mountedCombatData",  // Mounted combat data
            "environment",        // Environment data
            "characterData",      // Character data
            "mountedCombatState"  // Mounted combat state
        };
        
        outputs = {
            "mountedCombatStatus",  // Mounted combat status
            "mountedCombatMetrics"  // Mounted combat performance metrics
        };
    }
};

class MountedCombat : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit MountedCombat(const MountedCombatInfo& info = MountedCombatInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void charge();
    void fire();
    void dodge();
    void evade();
    void defend();      // Defend against incoming attacks or danger
    void feignedDefend(); // Feigned defense to avoid being attacked or danger
    void lead();
    void follow();
    void idle();
    void retreat();     // Retreat from combat or danger
    void feignedRetreat();
    void callFormation();
    void joinFormation();
    void dismount();
    void mount();
    void processMountedCombat();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MountedCombat() = default;     // Default destructor
};

} // namespace hd
