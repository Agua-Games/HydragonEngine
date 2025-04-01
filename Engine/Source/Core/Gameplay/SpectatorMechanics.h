/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpectatorMechanics.h
 * @brief Header file for the SpectatorMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpectatorMechanics is a class that represents spectator mechanics in Hydragon.
 * - It is used to represent any spectator mechanics in the game world, such as camera, replay, etc.
 * - It supports interactive features, such as camera, replay, and betting. Also supports two-way messaging with other spectator mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SpectatorMechanicsInfo : public NodeInfo {
    SpectatorMechanicsInfo() {
        nodeType = "Gameplay/SpectatorMechanics";
        
        inputs = {
            "spectatorData",        // Spectator data
            "environment",          // Environment data
            "characterData",        // Character data
            "spectatorState"        // Spectator state
        };
        
        outputs = {
            "spectatorStatus",      // Spectator status
            "spectatorMetrics"      // Spectator performance metrics
        };
    }
};

class SpectatorMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpectatorMechanics(const SpectatorMechanicsInfo& info = SpectatorMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable spectatorData;    // Spectator data, such as camera, replay, etc.
    DataTable spectatorState;    // Spectator state, such as camera, replay, etc.
    int maxSpectators = 0;
    bool enableFreeCamera = false;
    bool playerPOV = false;
    float replayBuffer = 0.0f;

    // === Processing ===
    void processNode() override { 
        
    }
    void enter();
    void exit();
    void interact();
    void message();
    void processSpectatorMechanics();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpectatorMechanics() = default;     // Default destructor
};

}