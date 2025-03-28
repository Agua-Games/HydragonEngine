/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StealthMechanics.h
 * @brief Header file for the StealthMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - StealthMechanics is a class that represents stealth mechanics in Hydragon.
 * - It is used to represent any stealth mechanics in the game world, such as visibility, sound, etc.
 * - It supports interactive features, such as hiding, sneaking, and blending in. Also supports two-way messaging with other stealth mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Ability.h"

namespace hd {

struct StealthMechanicsInfo : public NodeInfo {
    StealthMechanicsInfo() {
        nodeType = "Gameplay/StealthMechanics";
        
        inputs = {
            "stealthData",        // Stealth data
            "environment",        // Environment data
            "characterData",      // Character data
            "stealthState"        // Stealth state
        };
        
        outputs = {
            "stealthStatus",      // Stealth status
            "stealthMetrics"      // Stealth performance metrics
        };
    }
};

/**
 * @class StealthMechanics
 * @brief Class representing stealth mechanics in Hydragon.
 */
class StealthMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit StealthMechanics(const StealthMechanicsInfo& info = StealthMechanicsInfo())
        : Node(info) {}    // Default constructor
    void initialize() override {}
    void load() override {}

    // Set default values
    float visibility = 0.0f;
    float soundEmission = 0.0f;
    float detectionRange = 0.0f;
    float detectionAngle = 0.0f;
    float detectionHeight = 0.0f;
    float detectionAmount = 0.0f;
    float detectionThreshold = 0.0f;
    float detectionCooldown = 0.0f;

    // === Processing ===
    void hide();
    void sneak();
    void blendIn();
    void detect();
    void processStealth();
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~StealthMechanics() = default;     // Default destructor
};

} // namespace hd
