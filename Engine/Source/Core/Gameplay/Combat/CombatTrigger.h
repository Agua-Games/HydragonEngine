/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatTrigger.h
 * @brief Header file for the CombatTrigger class.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatTrigger is a class that represents a combat trigger in Hydragon.
 * - It is used to represent any combat trigger in the game world, such as combat zones, combat events, etc.
 * - It supports interactive features, such as triggering combat events and responding to combat events. Also supports two-way messaging with other combat triggers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Trigger.h"

namespace hd {

struct CombatTriggerType {
    enum class Type {
        CombatZone,
        CombatEvent,
        Custom
    };
};

struct CombatTriggerInfo : public TriggerInfo {
    CombatTriggerInfo() {
        nodeType = "Gameplay/CombatTrigger";
        
        inputs = {
            "triggerType",     // Type of combat trigger (combat zone, combat event, etc.)
            "triggerData",     // Combat trigger data
            "environment",     // Environment data
            "characterData",   // Character data
            "triggerState"     // Combat trigger state
        };
        
        outputs = {
            "triggerStatus",   // Combat trigger status
            "triggerMetrics"   // Combat trigger performance metrics
        };
    }
};

class CombatTrigger : public Trigger {
public:
    // === Allocation, Initialization, Loading ===
    explicit CombatTrigger(const CombatTriggerInfo& info = CombatTriggerInfo())
        : Trigger(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    std::string triggerType = "none";
    bool triggered = false;
    bool active = false; // Added for combat trigger activation/deactivation logic
    DataTable triggerData = DataTable();

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void OnEnter(const std::string& objectName) override;
    void OnExit(const std::string& objectName) override;
    void OnStay(const std::string& objectName) override;
    void unload() override {}
    void cleanup() override {}
    ~CombatTrigger() = default;     // Default destructor
};

} // namespace hd
