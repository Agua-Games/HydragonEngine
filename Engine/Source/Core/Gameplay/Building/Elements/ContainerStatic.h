/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ContainerStatic.h
 * @brief Header file for the ContainerStatic class.
 * 
 * ARCHITECTURAL NOTES:
 * - ContainerStatic is a class that represents a static container in Hydragon.
 * - It is used to represent any static container in the game world, such as water tanks, water mirrors, pools, chemical tanks, etc.
 * - It supports interactive features, such as filling, emptying, and overflowing. Also supports two-way messaging with other containers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"
#include "PhysicsFields.h"

namespace hd {

struct ContainerStaticInfo : public BuildingElementInfo {
    ContainerStaticInfo() {
        nodeType = "Gameplay/ContainerStatic";
        
        inputs = {
            "containerStaticType",  // Type of container (water tank, etc.)
            "containerStaticData",  // Container data
            "environment", // Environment data
            "characterData", // Character data
            "containerStaticState" // Container state
        };
        
        outputs = {
            "containerStaticStatus", // Container status
            "containerStaticMetrics" // Container performance metrics
        };
    }
};

class ContainerStatic : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit ContainerStatic(const ContainerStaticInfo& info = ContainerStaticInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    
    }
    void update();
    
    void fill();
    void empty();
    void overflow();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ContainerStatic() = default;     // Default destructor
};

} // namespace hd
