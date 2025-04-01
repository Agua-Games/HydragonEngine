/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DetonationMechanics.h
 * @brief Header file for the DetonationMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - DetonationMechanics is a class that represents detonation mechanics in Hydragon.
 * - It is used to represent any detonation mechanics in the game world, such as grenades, mines, etc.
 * - It supports interactive features, such as detonating, triggering, and deploying. Also supports two-way messaging with other detonation mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "RadioChannel.h"
#include "DataTable.h"
#include "Device.h"

namespace hd {

struct DetonationMechanicsInfo : public NodeInfo {
    DetonationMechanicsInfo() {
        nodeType = "Gameplay/DetonationMechanics";
        
        inputs = {
            "detonationData",        // Detonation data
            "environment",           // Environment data
            "characterData",         // Character data
            "detonationState"        // Detonation state
        };
        
        outputs = {
            "detonationStatus",      // Detonation status
            "detonationMetrics"      // Detonation performance metrics
        };
    }
};

class DetonationMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit DetonationMechanics(const DetonationMechanicsInfo& info = DetonationMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float timer = 0.0f;
    bool remote = false;

    // === Processing ===
    void processNode() override { }
    void detonate();
    void trigger();
    void deploy();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DetonationMechanics() = default;     // Default destructor
};

} // namespace hd
