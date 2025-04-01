/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Horse.h
 * @brief Header file for the Horse class.
 * 
 * ARCHITECTURAL NOTES:
 * - Horse is a class that represents a horse in Hydragon.
 * - It is used to represent any horse in the game world, such as stallions, mares, geldings, etc.
 * - It supports interactive features, such as galloping, trotting, and neighing. Also supports two-way messaging with other horses, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"
#include "Creature.h"
#include "CreatureTypes.h"
#include "HorseMechanics.h"

namespace hd {

enum class HorseType {
    Stallion,
    Mare,
    Gelding,
    Other
};

enum class HorseBreed {
    Arabian,
    Thoroughbred,
    QuarterHorse,
    Mustang,
    Clydesdale,
    Shetland,
    Other
};

struct HorseInfo : public CreatureInfo {
    HorseInfo() {
        nodeType = "Gameplay/Horse";
        
        inputs = {
            "horseType",        // Type of horse (stallion, mare, gelding, etc.)
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

class Horse : public Creature {
public:
    // === Allocation, Initialization, Loading ===
    explicit Horse(const HorseInfo& info = HorseInfo())
        : Creature(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    HorseType type = HorseType::Other;
    HorseBreed breed = HorseBreed::Other;
    float courage = 0.0f;
    float loyalty = 0.0f;
    float speed = 0.0f;
    float health = 0.0f;
    float stamina = 0.0f;
    DataTable horseData;

    // === Processing ===
    void processNode() override {
 
    }
    void update();
    void setBreed();
    void gallop();     // Gallop at a fast speed, like a horse in a race.
    void trot();       // Trot at a moderate speed, like a horse in a race.
    void neigh();      // Neigh at a slow speed, like a horse in a race.
    void herd();       // Herd a group of horses together.
    void flock();      // Flock a group of horses together.
    void processHorse();    // Process the horse.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Horse() = default;     // Default destructor
};

} // namespace hd