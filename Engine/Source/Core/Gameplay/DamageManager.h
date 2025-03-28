/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DamageManager.h
 * @brief Header file for the DamageManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - DamageManager is a singleton class that manages the damage in the game.
 * - It is responsible for managing the damage in the game.
 * - It supports interactive features, such as dealing and receiving damage. Also supports two-way messaging with other damage zones, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "DamageZone.h"

namespace hd {

struct DamageManagerInfo : public NodeInfo {
    DamageManagerInfo() {
        nodeType = "Gameplay/DamageManager";
        
        inputs = {
            "damageData",      // Damage data
            "environment",     // Environment data
            "characterData",   // Character data
            "damageState"      // Damage state
        };
        
        outputs = {
            "damageStatus",    // Damage status
            "damageMetrics"    // Damage performance metrics
        };
    }
};

class DamageManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DamageManager(const DamageManagerInfo& info = DamageManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void addDamageZone(DamageZone* zone);
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DamageManager() = default;     // Default destructor
};

} // namespace hd
