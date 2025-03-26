/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HazardZone.h
 * @brief Header file for the HazardZone class.
 * 
 * ARCHITECTURAL NOTES:
 * - HazardZone is a class that represents a hazard zone in Hydragon.
 * - It is used to represent any hazard zone in the game world, such as radiation zones, toxic zones, etc.
 * - It supports interactive features, such as detecting and responding to hazards. Also supports two-way messaging with other hazard zones, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct HazardZoneInfo : public NodeInfo {
    HazardZoneInfo() {
        nodeType = "Gameplay/HazardZone";
        
        inputs = {
            "hazardType",      // Type of hazard
            "hazardData",      // Hazard data
            "environment",     // Environment data
            "characterData",   // Character data
            "hazardState"      // Hazard state
        };
        
        outputs = {
            "hazardStatus",    // Hazard status
            "hazardMetrics"    // Hazard performance metrics
        };
    }
};

class HazardZone : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit HazardZone(const HazardZoneInfo& info = HazardZoneInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~HazardZone() = default;     // Default destructor
};

} // namespace hd