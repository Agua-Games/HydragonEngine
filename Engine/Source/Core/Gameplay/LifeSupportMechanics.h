/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LifeSupportMechanics.h
 * @brief Header file for the LifeSupportMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - LifeSupportMechanics is a class that represents life support mechanics in Hydragon.
 * - It is used to represent any life support mechanics in the game world, such as oxygen, water, food, etc.
 * - It supports interactive features, such as consuming, regenerating, and monitoring. Also supports two-way messaging with other life support mechanics, environment, character, etc.
 */
#pragma once
#include <vector>
#include "Engine.h"
#include "Node.h"
#include "EnvironmentManager.h"

namespace hd {

enum class LifeSupportType {
    Oxygen,
    Water,
    Food,
    Custom
};

struct LifeSupportMechanicsInfo : public NodeInfo {
    LifeSupportMechanicsInfo() {
        nodeType = "Gameplay/LifeSupportMechanics";
        
        inputs = {
            "lifeSupportData",        // Life support data
            "environment",            // Environment data
            "characterData",          // Character data
            "lifeSupportState"        // Life support state
        };
        
        outputs = {
            "lifeSupportStatus",      // Life support status
            "lifeSupportMetrics"      // Life support performance metrics
        };
    }
};

class LifeSupportMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit LifeSupportMechanics(const LifeSupportMechanicsInfo& info = LifeSupportMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    std::vector<LifeSupportType> lifeSupportTypes;  // Types of life support (oxygen, water, food, etc.) supported by this life support mechanics node. Default: empty vector.
    float regenerationRate = 0.0f;
    float consumptionRate = 0.0f;
    float lifeSignal = 0.0f;
    float efficiency = 0.0f;

    // === Processing ===
    void processNode() override { }
    void regenerate();
    void monitor();
    void processLifeSupportMechanics();
    void consume();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LifeSupportMechanics() = default;     // Default destructor
};

} // namespace hd
