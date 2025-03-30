/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DockingMechanics.h
 * @brief Header file for the DockingMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - DockingMechanics is a class that represents docking mechanics in Hydragon.
 * - It is used to represent any docking mechanics in the game world, such as docking, undocking, etc.
 * - It supports interactive features, such as docking, undocking, and docking stations. Also supports two-way messaging with other docking mechanics, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct DockingMechanicsInfo : public NodeInfo {
    DockingMechanicsInfo() {
        nodeType = "Gameplay/DockingMechanics";
        
        inputs = {
            "dockingData",        // Docking data
            "environment",        // Environment data
            "characterData",      // Character data
            "dockingState"        // Docking state
        };
        
        outputs = {
            "dockingStatus",      // Docking status
            "dockingMetrics"      // Docking performance metrics
        };
    }
};

class DockingMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DockingMechanics(const DockingMechanicsInfo& info = DockingMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void dock();
    void undock();
    void processDocking();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DockingMechanics() = default;     // Default destructor
};

} // namespace hd
