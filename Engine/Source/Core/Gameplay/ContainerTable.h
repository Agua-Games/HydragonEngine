/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ContainerTable.h
 * @brief Header file for the ContainerTable class.
 * 
 * ARCHITECTURAL NOTES:
 * - ContainerTable is a class that represents a container table in Hydragon.
 * - It is used to represent any container table in the game world, such as chests, crates, etc.
 * - It supports interactive features, such as adding, removing, and iterating over. Also supports two-way messaging with other container tables, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct ContainerTableInfo : public NodeInfo {
    ContainerTableInfo() {
        nodeType = "Gameplay/ContainerTable";
        
        inputs = {
            "containerType",  // Type of container (chest, crate, etc.)
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

class ContainerTable : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ContainerTable(const ContainerTableInfo& info = ContainerTableInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addItem(const std::string& item, float quantity);
    void removeItem(const std::string& item, float quantity);
    void processItems();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ContainerTable() = default;     // Default destructor
};

}