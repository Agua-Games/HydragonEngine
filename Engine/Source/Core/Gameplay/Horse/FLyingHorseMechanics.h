/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FLyingHorseMechanics.h
 * @brief Header file for the FLyingHorseMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - FLyingHorseMechanics is a class that inherits from WarHorseMechanics and represents flying horse mechanics in Hydragon.
 * - It is used to represent any flying horse mechanics like seen in flying horse games, like flying horse combat, flying horse training, flying horse breeding, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other flying horse mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "WarHorseMechanics.h"
#include "Ability.h"

namespace hd {

struct FLyingHorseMechanicsInfo : public NodeInfo {
    FLyingHorseMechanicsInfo() {
        nodeType = "Gameplay/FLyingHorseMechanics";
        
        inputs = {
            "flyingHorseData",        // Flying horse data
            "environment",            // Environment data
            "characterData",          // Character data
            "flyingHorseState"        // Flying horse state
        };
        
        outputs = {
            "flyingHorseStatus",      // Flying horse status
            "flyingHorseMetrics"      // Flying horse performance metrics
        };
    }
};

class FLyingHorseMechanics : public WarHorseMechanics {
public:
    // === Allocation, Initialization, Loading === 
    explicit FLyingHorseMechanics(const FLyingHorseMechanicsInfo& info = FLyingHorseMechanicsInfo())
        : WarHorseMechanics(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void takeOff();
    void fly();
    void glide(); // Fly without using any energy.
    void soar();
    void evade();
    void hover();
    void land();
    void processFlyingHorse();
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FLyingHorseMechanics() = default;     // Default destructor
};

} // namespace hd