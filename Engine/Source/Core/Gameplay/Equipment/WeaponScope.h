/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WeaponScope.h
 * @brief Header file for the WeaponScope class.
 * 
 * ARCHITECTURAL NOTES:
 * - WeaponScope is a class that represents a weapon scope in Hydragon.
 * - It is used to represent any weapon scope in the game world, such as scopes, sights, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other weapon scopes, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Equipment.h"

namespace hd {

struct WeaponScopeInfo : public NodeInfo {
    WeaponScopeInfo() {
        nodeType = "Gameplay/WeaponScope";
        
        inputs = {
            "scopeType",  // Type of scope (red dot, reflex, etc.)
            "scopeData",  // Scope data
            "environment", // Environment data
            "characterData", // Character data
            "scopeState" // Scope state
        };
        
        outputs = {
            "scopeStatus", // Scope status
            "scopeMetrics" // Scope performance metrics
        };
    }
};

class WeaponScope : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit WeaponScope(const WeaponScopeInfo& info = WeaponScopeInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processWeaponScope();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WeaponScope() = default;     // Default destructor
};

} // namespace hd
