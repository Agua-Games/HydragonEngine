/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SurveillanceMechanics.h
 * @brief Header file for the SurveillanceMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - SurveillanceMechanics is a class that represents surveillance mechanics in Hydragon.
 * - It is used to represent any surveillance mechanics in the game world, such as cameras, sensors, etc.
 * - It supports interactive features, such as detecting and responding to security threats. Also supports two-way messaging with other surveillance and security systems,
 * environment, character, etc.
 */
#pragma once
#include "Core.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SurveillanceMechanicsInfo : public NodeInfo {
    SurveillanceMechanicsInfo() {
        nodeType = "Gameplay/SurveillanceMechanics";
        
        inputs = {
            "surveillanceData",        // Surveillance data
            "environment",             // Environment data
            "characterData",           // Character data
            "surveillanceState"        // Surveillance state
        };
        
        outputs = {
            "surveillanceStatus",      // Surveillance status
            "surveillanceMetrics"      // Surveillance performance metrics
        };
    }
};

class SurveillanceMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SurveillanceMechanics(const SurveillanceMechanicsInfo& info = SurveillanceMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    bool active = false;
    bool analog = false;
    bool digital = false;
    bool online = false;
    DataTable surveillanceMechanicsData;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SurveillanceMechanics() = default;     // Default destructor
};

} // namespace hd
