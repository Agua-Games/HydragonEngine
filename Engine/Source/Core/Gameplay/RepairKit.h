/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RepairKit.h
 * @brief Header file for the RepairKit class.
 * 
 * ARCHITECTURAL NOTES:
 * - RepairKit is a class that represents a repair kit in Hydragon.
 * - It is used to represent any repair kit in the game world, such as medical kits, repair kits, etc.
 * - It supports interactive features, such as healing, repairing, and upgrading. Also supports two-way messaging with other repair kits, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct RepairKitInfo : public EquipmentInfo {
    enum class Type {
        Medical,
        Repair,
        Upgrade,
        Custom
    };

    float health = 100.0f;      // Health of the repair kit
    float durability = 100.0f;   // Durability of the repair kit
    float upgradeLevel = 1.0f;   // Upgrade level of the repair kit
    float healAmount = 10.0f;   // Amount of health the repair kit can heal
    float repairAmount = 10.0f;  // Amount of durability the repair kit can repair
    float repairTime = 10.0f;    // Time it takes to repair something with the repair kit
    bool requiresCrew = false;   // Whether the repair kit requires a crew to use

    RepairKitInfo() {
        nodeType = "Gameplay/RepairKit";
        
        inputs = {
            "repairKitType",    // Type of repair kit (medical, repair, etc.)
            "health",           // Health of the repair kit
            "durability",       // Durability of the repair kit
            "upgradeLevel",     // Upgrade level of the repair kit
            "healAmount",      // Amount of health the repair kit can heal
            "repairAmount",    // Amount of durability the repair kit can repair
            "repairTime",      // Time it takes to repair something with the repair kit
            "requiresCrew",     // Whether the repair kit requires a crew to use
            "repairKitData",    // Repair kit data
            "environment",      // Environment data
            "characterData",    // Character data
            "repairKitState"    // Repair kit state
        };
        
        outputs = {
            "repairKitStatus",  // Repair kit status
            "repairKitMetrics"  // Repair kit performance metrics
        };
    }
};

class RepairKit : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit RepairKit(const RepairKitInfo& info = RepairKitInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values for repair kit info struct
    RepairKitInfo repairKitInfo = {};  // Initialize repair kit info struct with default values

    // === Processing ===
    void processNode() override {
 
    }
    void heal();       // to heal the character, vehicle, building, etc.
    void repair();     // to repair the character, vehicle, building, etc.
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RepairKit() = default;     // Default destructor
};

} // namespace hd

