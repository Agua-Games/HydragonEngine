/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RaceMechanics.h
 * @brief Header file for the RaceMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - RaceMechanics is a class that represents race mechanics in Hydragon.
 * - It is used to represent any race mechanics in the game world, such as race, speed, etc.
 * - It supports interactive features, such as racing, speed, and acceleration. Also supports two-way messaging with other race mechanics, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"

namespace hd {

struct RaceMechanicsInfo : public NodeInfo {
    RaceMechanicsInfo() {
        nodeType = "Gameplay/RaceMechanics";
        
        inputs = {
            "raceMechanicsData",// Race mechanics data
            "environment",       // Environment data
            "characterData",     // Character data
            "raceMechanicsState"// Race mechanics state
        };
        
        outputs = {
            "raceMechanicsStatus",// Race mechanics status
            "raceMechanicsMetrics"// Race mechanics performance metrics
        };
    }
};

class RaceMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RaceMechanics(const RaceMechanicsInfo& info = RaceMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable raceMechanicsData;
    float timeProgress = 0.0f;
    float speed = 0.0f;
    bool dynamicDifficulty = false;
    float minDifficulty = 0.0f;
    float maxDifficulty = 0.0f;
    float adaptationRate = 0.0f;
    bool dynamicWeather = false;
    DataTable raceMechanicsState;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RaceMechanics() = default;     // Default destructor
};

} // namespace hd

