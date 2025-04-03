/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Garrison.h
 * @brief Header file for the Garrison class.
 * 
 * ARCHITECTURAL NOTES:
 * - Garrison is a class that represents a garrison in Hydragon.
 * - It is used to represent any garrison in the game world, such as fortresses, castles, etc.
 * - It supports interactive features, such as garrison management, garrison communication, garrison objectives, etc. Also supports two-way messaging with other garrisons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"

namespace hd {

struct GarrisonInfo : public BuildingInfo {
    GarrisonInfo() {
        nodeType = "Gameplay/Garrison";
        
        inputs = {
            "garrisonData",        // Garrison data
            "environment",         // Environment data
            "characterData",       // Character data
            "garrisonState"        // Garrison state
        };
        
        outputs = {
            "garrisonStatus",      // Garrison status
            "garrisonMetrics"      // Garrison performance metrics
        };
    }
};

class Garrison : public Building {
public:
    // === Allocation, Initialization, Loading === 
    explicit Garrison(const GarrisonInfo& info = GarrisonInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addGuard(const std::string& guardName);
    void removeGuard(const std::string& guardName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Garrison() = default;     // Default destructor
};

} // namespace hd
