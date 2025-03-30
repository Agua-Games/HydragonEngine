/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Harbor.h
 * @brief Header file for the Harbor class.
 * 
 * ARCHITECTURAL NOTES:
 * - Harbor is a class that represents a harbor in Hydragon.
 * - It is used to represent any harbor in the game world, such as ports, docks, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other harbors, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Station.h"
#include "Vehicle.h"

namespace hd {

struct HarborInfo : public StationInfo {
    HarborInfo() {
        nodeType = "Gameplay/Harbor";
        
        inputs = {
            "harborType",      // Type of harbor (port, dock, etc.)
            "harborData",      // Harbor data
            "environment",     // Environment data
            "characterData",   // Character data
            "harborState"      // Harbor state
        };
        
        outputs = {
            "harborStatus",    // Harbor status
            "harborMetrics"    // Harbor performance metrics
        };
    }
};

class Harbor : public Station {
    public:
    // === Allocation, Initialization, Loading ===
    explicit Harbor(const HarborInfo& info = HarborInfo())
        : Station(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void enterHarbor();
    void exitHarbor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Harbor() = default;     // Default destructor
};

} // namespace hd