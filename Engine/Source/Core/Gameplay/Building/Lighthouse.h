/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Lighthouse.h
 * @brief Header file for the Lighthouse class.
 * 
 * ARCHITECTURAL NOTES:
 * - Lighthouse is a class that represents a lighthouse in Hydragon.
 * - It is used to represent any lighthouse in the game world, such as lighthouses, beacons, etc.
 * - It supports interactive features, such as lighting, dimming, and flickering. Also supports two-way messaging with other lighthouses, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"
#include "Light.h"

namespace hd {

struct LighthouseInfo : public BuildingInfo {
    LighthouseInfo() {
        nodeType = "Gameplay/Lighthouse";
        
        inputs = {
            "lighthouseType",  // Type of lighthouse (lighthouse, beacon, etc.)
            "lighthouseData",  // Lighthouse data
            "environment",     // Environment data
            "characterData",   // Character data
            "lighthouseState"  // Lighthouse state
        };
        
        outputs = {
            "lighthouseStatus",  // Lighthouse status
            "lighthouseMetrics"  // Lighthouse performance metrics
        };
    }
};

class Lighthouse : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Lighthouse(const LighthouseInfo& info = LighthouseInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processLighthouse();
    void processNode() override {
        processLighthouse(); 
    }
    void light();
    void dim();
    void flicker();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Lighthouse() = default;     // Default destructor
};

} // namespace hd
