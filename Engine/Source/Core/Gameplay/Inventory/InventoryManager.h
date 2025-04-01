/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InventoryManager.h
 * @brief Header file for the InventoryManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - InventoryManager is a singleton class that manages the inventories in the game.
 * - It is responsible for managing the inventories in the game.
 * - It supports interactive features, such as adding, removing, and iterating over. Also supports two-way messaging with other inventory, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "Inventory.h"
#include "DataTable.h"

namespace hd {

struct InventoryManagerInfo : public NodeInfo {
    InventoryManagerInfo() {
        NodeType = "Gameplay/InventoryManager";
        inputs = {
            "Inventory" // Inventory data
        };        
        outputs = {
            "InventoryStatus",  // Inventory status
            "InventoryMetrics"  // Inventory performance metrics
        };
    }
};

class InventoryManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit InventoryManager(const InventoryManagerInfo& info = InventoryManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addInventory(Inventory* inventory);
    void removeInventory(Inventory* inventory);
    void queryInventory(Inventory* inventory);
    void processInventory();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~InventoryManager() = default;     // Default destructor
};

} // namespace hd
