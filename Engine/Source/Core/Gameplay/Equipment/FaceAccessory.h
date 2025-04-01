/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FaceAccessory.h
 * @brief Header file for the FaceAccessory class.
 * 
 * ARCHITECTURAL NOTES:
 * - FaceAccessory is a class that represents a face accessory in Hydragon.
 * - It is used to represent any face accessory in the game world, such as glasses, masks, eye patches, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other face accessories, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct FaceAccessoryInfo : public EquipmentInfo {
    FaceAccessoryInfo() {
        nodeType = "Gameplay/FaceAccessory";
        
        inputs = {
            "faceAccessoryType",  // Type of face accessory (glasses, mask, eye patch, etc.)
            "faceAccessoryData",  // Face accessory data
            "environment",        // Environment data
            "characterData",      // Character data
            "faceAccessoryState"  // Face accessory state
        };
        
        outputs = {
            "faceAccessoryStatus",  // Face accessory status
            "faceAccessoryMetrics"  // Face accessory performance metrics
        };
    }
};

class FaceAccessory : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit FaceAccessory(const FaceAccessoryInfo& info = FaceAccessoryInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void equip();
    void unequip();
    void upgrade();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FaceAccessory() = default;     // Default destructor
};

} // namespace hd
