/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file IndoorMechanics.h
 * @brief Header file for the IndoorMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - IndoorMechanics is a class that represents indoor mechanics in Hydragon.
 * - It is used to represent any indoor mechanics in the game world, such as lighting, sound, etc.
 * - It supports interactive features, such as lighting, sound, and ambiance. Also supports two-way messaging with other indoor mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct IndoorMechanicsInfo : public NodeInfo {
    IndoorMechanicsInfo() {
        nodeType = "Gameplay/IndoorMechanics";
        
        inputs = {
            "indoorData",        // Indoor data
            "environment",       // Environment data
            "characterData",     // Character data
            "indoorState"        // Indoor state
        };
        
        outputs = {
            "indoorStatus",      // Indoor status
            "indoorMetrics"      // Indoor performance metrics
        };
    }
};

class IndoorMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit IndoorMechanics(const IndoorMechanicsInfo& info = IndoorMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addFeature(const std::string& featureName);
    void removeFeature(const std::string& featureName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~IndoorMechanics() = default;     // Default destructor
};

} // namespace hd
