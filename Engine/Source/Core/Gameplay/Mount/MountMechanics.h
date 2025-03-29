/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MountMechanics.h
 * @brief Header file for the MountMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - MountMechanics is a class that represents mount mechanics in Hydragon.
 * - It is used to represent any mount mechanics in the game world, such as horses, camels, etc.
 * - It supports interactive features, such as mounting, dismounting, and riding. Also supports two-way messaging with other mount mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Math.h"
#include "Mount.h"

namespace hd {

struct MountMechanicsInfo : public NodeInfo {
    MountMechanicsInfo() {
        nodeType = "Gameplay/MountMechanics";
        
        inputs = {
            "mountData",        // Mount data
            "environment",      // Environment data
            "characterData",    // Character data
            "mountState"        // Mount state
        };
        
        outputs = {
            "mountStatus",      // Mount status
            "mountMetrics"      // Mount performance metrics
        };
    }
};

class MountMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit MountMechanics(const MountMechanicsInfo& info = MountMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    // Need to decide, based on architecture, if some of these should go into the mount entity, not here
    void enableHalterGuidance(bool enable);
    void moveTo(const vec3& target);
    void moveToRider();
    void flee();
    void leadHerd();
    void follow();
    void idle();
    void processMount();
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MountMechanics() = default;     // Default destructor
};

} // namespace hd
