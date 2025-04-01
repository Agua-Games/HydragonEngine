/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Plantation.h
 * @brief Header file for the Plantation class.
 * 
 * ARCHITECTURAL NOTES:
 * - Plantation is a class that represents a plantation in Hydragon.
 * - It is used to represent any plantation in the game world, such as farms, orchards, etc.
 * - It supports interactive features, such as planting, harvesting, and managing crops. Also supports two-way messaging with other plantations, environment, character, etc.
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

struct PlantationInfo : public VegetationInfo {
    PlantationInfo() {
        nodeType = "Gameplay/Plantation";
        
        inputs = {
            "plantationType",    // Type of plantation (farm, orchard, etc.)
            "plantationData",    // Plantation data
            "environment",       // Environment data
            "characterData",     // Character data
            "plantationState"    // Plantation state
        };
        
        outputs = {
            "plantationStatus",  // Plantation status
            "plantationMetrics"  // Plantation performance metrics
        };
    }
};

class Plantation : public Vegetation {
public:
    // === Allocation, Initialization, Loading ===
    explicit Plantation(const PlantationInfo& info = PlantationInfo())
        : Vegetation(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processPlantation();
    void processNode() override {
        processPlantation(); 
    }
    void plant();
    void harvest();
    void manage();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Plantation() = default;     // Default destructor
};

} // namespace hd
