/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LaunchPlatform.h
 * @brief Header file for the LaunchPlatform class.
 * 
 * ARCHITECTURAL NOTES:
 * - LaunchPlatform is a class that represents a launch platform in Hydragon.
 * - It is used to represent any launch platform in the game world, such as launch pads, launch sites, etc.
 * - It supports interactive features, such as launching, aborting, and monitoring. Also supports two-way messaging with other launch platforms, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"

namespace hd {

struct LaunchPlatformInfo : public BuildingInfo {
    LaunchPlatformInfo() {
        nodeType = "Gameplay/LaunchPlatform";
        
        inputs = {
            "launchPlatformType",  // Type of launch platform (launch pad, launch site, etc.)
            "launchPlatformData",  // Launch platform data
            "environment",         // Environment data
            "characterData",       // Character data
            "launchPlatformState"  // Launch platform state
        };
        
        outputs = {
            "launchPlatformStatus",  // Launch platform status
            "launchPlatformMetrics"  // Launch platform performance metrics
        };
    }
};

class LaunchPlatform : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit LaunchPlatform(const LaunchPlatformInfo& info = LaunchPlatformInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void launch();
    void abort();
    void monitor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LaunchPlatform() = default;     // Default destructor
};

} // namespace hd
