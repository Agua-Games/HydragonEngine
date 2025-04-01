/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SecurityMechanics.h
 * @brief Header file for the SecurityMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - SecurityMechanics is a class that represents security mechanics in Hydragon.
 * - Generally related to SurveillanceMechanics, used together with it, but its responsibility is different. It is used to represent any security mechanics in the game world
 * in therms of security measures, actions, like doors that shut and lock, alarms, watertight chambers for space stations, subs, dispatch commands to guards, etc.
 * - It supports interactive features, such as detecting and responding to security threats. Also supports two-way messaging with other surveillance and security systems,
 * environment, character, etc.
 */
#pragma once
#include "Core.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SecurityMechanicsInfo : public NodeInfo {
    SecurityMechanicsInfo() {
        nodeType = "Gameplay/SecurityMechanics";
        
        inputs = {
            "securityData",        // Security data
            "environment",         // Environment data
            "characterData",       // Character data
            "securityState"        // Security state
        };
        
        outputs = {
            "securityStatus",      // Security status
            "securityMetrics"      // Security performance metrics
        };
    }
};

class SecurityMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SecurityMechanics(const SecurityMechanicsInfo& info = SecurityMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    bool active = false;
    bool analog = false;
    bool digital = false;
    bool online = false;
    DataTable securityMechanicsData;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SecurityMechanics() = default;     // Default destructor
};

} // namespace hd
