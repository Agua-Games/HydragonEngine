/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceSuit.h
 * @brief Header file for the SpaceSuit class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceSuit is a class that represents a space suit in Hydragon.
 * - It is used to represent any space suit in the game world, such as space suits, space helmets, etc.
 * - It supports interactive features, such as equipping, unequipping, and using. Also supports two-way messaging with other space suits, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Equipment.h"

namespace hd {

struct SpaceSuitInfo : public EquipmentInfo {
    SpaceSuitInfo() {
        nodeType = "Gameplay/SpaceSuit";
        
        inputs = {
            "spaceSuitType",  // Type of space suit (space suit, space helmet, etc.)
            "spaceSuitData",  // Space suit data
            "environment",    // Environment data
            "characterData",  // Character data
            "spaceSuitState"  // Space suit state
        };
        
        outputs = {
            "spaceSuitStatus",  // Space suit status
            "spaceSuitMetrics"  // Space suit performance metrics
        };
    }
};

class SpaceSuit : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpaceSuit(const SpaceSuitInfo& info = SpaceSuitInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void equip();
    void unequip();
    void use();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceSuit() = default;     // Default destructor
};

} // namespace hd
