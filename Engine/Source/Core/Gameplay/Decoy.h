/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Decoy.h
 * @brief Header file for the Decoy class.
 * 
 * ARCHITECTURAL NOTES:
 * - Decoy is a class that represents a decoy in Hydragon.
 * - It is used to represent any decoy in the game world, such as smoke grenades, flashbangs, etc.
 * - It supports interactive features, such as throwing, deploying, and detonating. Also supports two-way messaging with other decoys, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Prop.h"
#include "DataTable.h"

namespace hd {

struct DecoyInfo : public PropInfo {
    DecoyInfo() {
        nodeType = "Gameplay/Decoy";
        
        inputs = {
            "decoyType",      // Type of decoy (smoke, flashbang, etc.)
            "decoyData",      // Decoy data
            "environment",    // Environment data
            "characterData",  // Character data
            "decoyState"      // Decoy state
        };
        
        outputs = {
            "decoyStatus",    // Decoy status
            "decoyMetrics"    // Decoy performance metrics
        };
    }
};

class Decoy : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Decoy(const DecoyInfo& info = DecoyInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void throwDecoy();
    void deployDecoy();
    void detonateDecoy();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Decoy() = default;     // Default destructor
};

} // namespace hd
