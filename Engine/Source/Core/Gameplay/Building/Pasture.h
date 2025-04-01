/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Pasture.h
 * @brief Header file for the Pasture class.
 * 
 * ARCHITECTURAL NOTES:
 * - Pasture is a class that represents a pasture in Hydragon.
 * - It is used to represent any pasture in the game world, such as grasslands, meadows, etc.
 * - It supports interactive features, such as grazing, herding, and managing livestock. Also supports two-way messaging with other pastures, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vegetation.h"
#include "EnvironmentManager.h"

namespace hd {

struct PastureInfo : public VegetationInfo {
    PastureInfo() {
        nodeType = "Gameplay/Pasture";
        
        inputs = {
            "pastureType",       // Type of pasture (grassland, meadow, etc.)
            "pastureData",       // Pasture data
            "environment",       // Environment data
            "characterData",     // Character data
            "pastureState"       // Pasture state
        };
        
        outputs = {
            "pastureStatus",     // Pasture status
            "pastureMetrics"     // Pasture performance metrics
        };
    }
};

class Pasture : public Vegetation {
public:
    // === Allocation, Initialization, Loading ===
    explicit Pasture(const PastureInfo& info = PastureInfo())
        : Vegetation(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processPasture();
    void processNode() override {
        processPasture(); 
    }
    void graze();
    void herd();
    void manage();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Pasture() = default;     // Default destructor
};

} // namespace hd
