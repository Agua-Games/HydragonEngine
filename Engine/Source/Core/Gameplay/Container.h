/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Container.h
 * @brief Header file for the Container class.
 * 
 * ARCHITECTURAL NOTES:
 * - Container is a class that represents a container in Hydragon.
 * - It is used to represent any container in the game world, such as backpacks, vests, etc.
 * - It supports interactive features, such as adding, removing, and iterating over. Also supports two-way messaging with other containers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct ContainerInfo : public NodeInfo {
    ContainerInfo() {
        nodeType = "Gameplay/Container";
        
        inputs = {
            "containerType",  // Type of container (backpack, vest, etc.)
            "containerData",  // Container data
            "environment",    // Environment data
            "characterData",  // Character data
            "containerState"  // Container state
        };
        
        outputs = {
            "containerStatus",  // Container status
            "containerMetrics"  // Container performance metrics
        };
    }
};

class Container : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Container(const ContainerInfo& info = ContainerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Container() = default;     // Default destructor
};

} // namespace hd
