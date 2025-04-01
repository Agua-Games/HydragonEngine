/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Furniture.h
 * @brief Header file for the Furniture class.
 * 
 * ARCHITECTURAL NOTES:
 * - Furniture is a class that represents a furniture in Hydragon.
 * - It is used to represent any furniture in the game world, such as tables, chairs, beds, etc.
 * - It supports interactive features, such as sitting, lying, and using. Also supports two-way messaging with other furniture, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Prop.h"
#include "DataTable.h"

namespace hd {

struct FurnitureInfo : public PropInfo {
    FurnitureInfo() {
        nodeType = "Gameplay/Furniture";
        
        inputs = {
            "furnitureType",  // Type of furniture (table, chair, bed, etc.)
            "furnitureData",  // Furniture data
            "environment",    // Environment data
            "characterData",  // Character data
            "furnitureState"  // Furniture state
        };
        
        outputs = {
            "furnitureStatus",    // Furniture status
            "furnitureMetrics"    // Furniture performance metrics
        };
    }
};

class Furniture : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Furniture(const FurnitureInfo& info = FurnitureInfo())
        : Prop(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processFurniture();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Furniture() = default;     // Default destructor
};

} // namespace hd
