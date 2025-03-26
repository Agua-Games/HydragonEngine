/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Submarine.h
 * @brief Header file for the Submarine class.
 * 
 * ARCHITECTURAL NOTES:
 * - Submarine is a class that represents a submarine in Hydragon.
 * - It is used to represent any submarine in the game world, such as nuclear submarines, diesel submarines, etc.
 * - It supports interactive features, such as diving, surfacing, and torpedo launching. Also supports two-way messaging with other submarines, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "AudioFile.h"

namespace hd {

struct SubmarineInfo : public NodeInfo {
    SubmarineInfo() {
        nodeType = "Gameplay/Submarine";
        
        inputs = {
            "submarineType",        // Type of submarine (nuclear, diesel, etc.)
            "submarineData",        // Submarine data
            "environment",          // Environment data
            "characterData",        // Character data
            "submarineState"        // Submarine state
        };
        
        outputs = {
            "submarineStatus",      // Submarine status
            "submarineMetrics"      // Submarine performance metrics
        };
    }
};

class Submarine : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Submarine(const SubmarineInfo& info = SubmarineInfo())
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
    ~Submarine() = default;     // Default destructor
};

} // namespace hd
