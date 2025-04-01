/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Item.h
 * @brief Header file for the Item class.
 * 
 * ARCHITECTURAL NOTES:
 * - Item is a class that represents an item in Hydragon, primarily used for the inventory.
 * - It is used to represent any item in the game world, such as weapons, armor, potions, etc.
 * - It supports interactive features, such as equipping, using, and dropping. Also supports two-way messaging with other items, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct ItemInfo : public NodeInfo {
    ItemInfo() {
        nodeType = "Gameplay/Item";
        
        inputs = {
            "itemType",        // Type of item (weapon, armor, potion, etc.)
            "itemData",        // Item data
            "environment",     // Environment data
            "characterData",   // Character data
            "itemState"        // Item state
        };
        
        outputs = {
            "itemStatus",      // Item status
            "itemMetrics"      // Item performance metrics
        };
    }
};

class Item : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Item(const ItemInfo& info = ItemInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {}
    void processItem();
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Item() = default;     // Default destructor
};

} // namespace hd
