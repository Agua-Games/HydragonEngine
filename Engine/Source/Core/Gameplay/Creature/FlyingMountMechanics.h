/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FlyingMountMechanics.h
 * @brief Header file for the FlyingMountMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - FlyingMountMechanics is a class that inherits from MountMechanics and represents flying mount mechanics in Hydragon.
 * - It is used to represent any flying mount mechanics in the game world, such as pegasus, griffins, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other flying mount mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "MountMechanics.h"

namespace hd {

struct FlyingMountMechanicsInfo : public MountMechanicsInfo {
    FlyingMountMechanicsInfo() {
        nodeType = "Gameplay/FlyingMountMechanics";
        
        inputs = {
            "flyingMountData",        // Flying mount data
            "environment",            // Environment data
            "characterData",          // Character data
            "flyingMountState"        // Flying mount state
        };
        
        outputs = {
            "flyingMountStatus",      // Flying mount status
            "flyingMountMetrics"      // Flying mount performance metrics
        };
    }
};

class FlyingMountMechanics : public MountMechanics {
public:
    // === Allocation, Initialization, Loading ===
    explicit FlyingMountMechanics(const FlyingMountMechanicsInfo& info = FlyingMountMechanicsInfo())
        : MountMechanics(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void fly();
    void hover();
    void land();
    void processFlyingMount();
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FlyingMountMechanics() = default;     // Default destructor
};

} // namespace hd
