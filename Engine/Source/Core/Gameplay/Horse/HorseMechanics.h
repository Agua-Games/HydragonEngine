/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HorseMechanics.h
 * @brief Header file for the HorseMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - HorseMechanics is a class that inherits from MountMechanics and represents horse mechanics in Hydragon.
 * - It is used to represent any horse mechanics like seen in horse games, like mount handling, horse-rider bonding, horse racing, herds with group behavior, flocking, etc.
 * - It supports interactive features, such as galloping, trotting, and neighing. Also supports two-way messaging with other horse mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "MountMechanics.h"

namespace hd {

struct HorseMechanicsInfo : public MountMechanicsInfo {
    HorseMechanicsInfo() {
        nodeType = "Gameplay/HorseMechanics";
        
        inputs = {
            "horseData",        // Horse data
            "environment",      // Environment data
            "characterData",    // Character data
            "horseState"        // Horse state
        };
        
        outputs = {
            "horseStatus",      // Horse status
            "horseMetrics"      // Horse performance metrics
        };
    }
};

class HorseMechanics : public MountMechanics {
public:
    // === Allocation, Initialization, Loading === 
    explicit HorseMechanics(const HorseMechanicsInfo& info = HorseMechanicsInfo())
        : MountMechanics(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void setBreed();
    void gallop();     // Gallop at a fast speed, like a horse in a race.
    void trot();       // Trot at a moderate speed, like a horse in a race.
    void neigh();      // Neigh at a slow speed, like a horse in a race.
    void herd();       // Herd a group of horses together.
    void flock();      // Flock a group of horses together.
    void processNode() override { }
    void update();
    void processHorse();    // Process the horse.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~HorseMechanics() = default;     // Default destructor
};

} // namespace hd
