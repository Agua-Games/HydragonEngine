/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Statue.h
 * @brief Header file for the Statue class.
 * 
 * ARCHITECTURAL NOTES:
 * - Statue is a class that represents a statue in Hydragon.
 * - It is used to represent any statue in the game world, such as sculptures, busts, etc.
 * - It supports interactive features, such as admiring, touching, and interacting with. Also supports two-way messaging with other statues, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Prop.h"
#include "DataTable.h"

namespace hd {

struct StatueInfo : public PropInfo {
    StatueInfo() {
        nodeType = "Gameplay/Statue";
        
        inputs = {
            "statueType",  // Type of statue (sculpture, bust, etc.)
            "statueData",  // Statue data
            "environment", // Environment data
            "characterData", // Character data
            "statueState" // Statue state
        };
        
        outputs = {
            "statueStatus", // Statue status
            "statueMetrics" // Statue performance metrics
        };
    }
};

class Statue : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Statue(const StatueInfo& info = StatueInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    
    }
    void update();
    
    void admire();
    void touch();
    void interact();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Statue() = default;     // Default destructor
};

} // namespace hd
