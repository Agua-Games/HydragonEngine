/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CoverMechanics.h
 * @brief Header file for the CoverMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - CoverMechanics is a class that represents cover mechanics in Hydragon.
 * - It is used to represent any cover mechanics in the game world, such as hiding, peeking, etc.
 * - It supports interactive features, such as hiding, peeking, and blending in. Also supports two-way messaging with other cover mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Math.h"

namespace hd {

struct CoverMechanicsInfo : public NodeInfo {
    CoverMechanicsInfo() {
        nodeType = "Gameplay/CoverMechanics";
        
        inputs = {
            "coverData",        // Cover data
            "environment",      // Environment data
            "characterData",    // Character data
            "coverState"        // Cover state
        };
        
        outputs = {
            "coverStatus",      // Cover status
            "coverMetrics"      // Cover performance metrics
        };
    }
};

/**
 * @class CoverMechanics
 * @brief Class representing cover mechanics in Hydragon.
 */
class CoverMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit CoverMechanics(const CoverMechanicsInfo& info = CoverMechanicsInfo())
        : Node(info) {}    // Default constructor
    void initialize() override {}
    void load() override {}

    // Set default values
    float coverHeight = 0.0f;
    float leanAmount = 0.0f;
    float peekAmount = 0.0f;
    float blendInAmount = 0.0f;
    float blindFireAccuracy = 0.0f;
    float blindFireExposure = 0.0f;
    float detectionAmount = 0.0f;
    float detectionRange = 0.0f;
    float detectionAngle = 0.0f;
    float detectionHeight = 0.0f;

    // === Processing ===
    void findCover();
    void hide();
    void takeCover();
    void crouch();
    void prone();
    void lean();
    void leaveCover();
    void peek();
    void blendIn();
    void blindFire(); // Blind fire is a special type of fire that is used when the character is in cover. It is used to simulate the character firing while being hidden. This is used for weapons that require a charge time before firing, such as bows and crossbows. Default: no charge.
    void detect();
    void processCover();
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CoverMechanics() = default;     // Default destructor
};

} // namespace hd
