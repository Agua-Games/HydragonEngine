/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Vine.h
 * @brief Header file for the Vine class.
 * 
 * ARCHITECTURAL NOTES:
 * - Vine is a class that represents a vine in Hydragon.
 * - It is used to represent any vine in the game world, such as grapes, hops, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other vines, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vegetation.h"

namespace hd {

struct VineInfo : public VegetationInfo {
    VineInfo() {
        nodeType = "Vegetation/Vine";  
        
        inputs = {
            "vineType",        // Type of vine (grapes, hops, etc.)
            "vineData",        // Vine data
            "environment",     // Environment data
            "characterData",   // Character data
            "vineState"        // Vine state
        };
        
        outputs = {
            "vineStatus",      // Vine status
            "vineMetrics"      // Vine performance metrics
        };
    }
};

class Vine : public Vegetation {
public:
    // === Allocation, Initialization, Loading ===
    explicit Vine(const VineInfo& info = VineInfo())
        : Vegetation(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processVine();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Vine() = default;     // Default destructor
};

} // namespace hd
