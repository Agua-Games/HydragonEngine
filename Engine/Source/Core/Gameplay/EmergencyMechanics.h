/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EmergencyMechanics.h
 * @brief Header file for the EmergencyMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - EmergencyMechanics is a class that represents emergency mechanics in Hydragon.
 * - It is used to represent any emergency mechanics in the game world, such as emergency response, emergency evacuation, etc.
 * - It supports interactive features, such as responding, evacuating, and communicating. Also supports two-way messaging with other emergency mechanics, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct EmergencyMechanicsInfo : public NodeInfo {
    EmergencyMechanicsInfo() {
        nodeType = "Gameplay/EmergencyMechanics";
        
        inputs = {
            "emergencyData",        // Emergency data
            "environment",          // Environment data
            "characterData",        // Character data
            "emergencyState"        // Emergency state
        };
        
        outputs = {
            "emergencyStatus",      // Emergency status
            "emergencyMetrics"      // Emergency performance metrics
        };
    }
};

class EmergencyMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit EmergencyMechanics(const EmergencyMechanicsInfo& info = EmergencyMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void respond();
    void evacuate();
    void communicate();
    void processEmergency();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EmergencyMechanics() = default;     // Default destructor
};

} // namespace hd
