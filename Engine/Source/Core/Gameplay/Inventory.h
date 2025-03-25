/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Inventory.h
 * @brief Header file for the Inventory class.
 * 
 * ARCHITECTURAL NOTES:
 * - Inventory is a class that handles inventory management.
 * - It is responsible for managing the inventory of items.
 * - It uses the Vulkan API for inventory management.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct InventoryInfo : public NodeInfo {
    InventoryInfo() {
        NodeType = "Gameplay/Inventory";
        
        inputs = {
            "Items",        // Array of items
            "ItemTypes",    // Item type definitions
            "ItemCounts",   // Item count for each type
            "ItemWeights",  // Item weight for each type
            "ItemValues",   // Item value for each type
            "ItemSlots",    // Inventory slots
            "ItemFilters",  // Filter definitions
            "Sorts"         // Sort definitions
        };
        
        outputs = {
            "Items",        // Array of items
            "ItemTypes",    // Item type definitions
            "ItemCounts",   // Item count for each type
            "ItemWeights",  // Item weight for each type
            "ItemValues",   // Item value for each type
            "ItemSlots",    // Inventory slots
            "ItemFilters",  // Filter definitions
            "Sorts"         // Sort definitions
        };
    }
};

class Inventory : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Inventory(const NodeInfo& info = NodeInfo())
        : Node(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // === Processing ===
    void processInventory();
    void processNodeGraph() override {
        processInventory(); 
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Inventory() = default;     // Default destructor
};

} // namespace hd
