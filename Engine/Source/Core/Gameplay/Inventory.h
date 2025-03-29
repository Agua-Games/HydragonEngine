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
#include "Item.h"

namespace hd {

struct InventoryInfo : public NodeInfo {
    InventoryInfo() {
        nodeType = "Gameplay/Inventory";
        
        inputs = {
            "items",        // Array of items
            "itemTypes",    // Item type definitions
            "itemCounts",   // Item count for each type
            "itemWeights",  // Item weight for each type
            "itemValues",   // Item value for each type
            "itemSlots",    // Inventory slots
            "itemFilters",  // Filter definitions
            "sorts"         // Sort definitions
        };
        
        outputs = {
            "items",        // Array of items
            "itemTypes",    // Item type definitions
            "itemCounts",   // Item count for each type
            "itemWeights",  // Item weight for each type
            "itemValues",   // Item value for each type
            "itemSlots",    // Inventory slots
            "itemFilters",  // Filter definitions
            "sorts"         // Sort definitions
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

    // Set default values
    std::vector<Item> items;
    std::unordered_map<std::string, ItemType> itemTypes;
    std::unordered_map<std::string, int> itemCounts;
    std::unordered_map<std::string, float> itemWeights;
    std::unordered_map<std::string, float> itemValues;
    std::vector<InventorySlot> itemSlots;
    std::vector<Filter> itemFilters;
    std::vector<Sort> sorts;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
        items = getInputValue<std::vector<Item>>("items");
        itemTypes = getInputValue<std::unordered_map<std::string, ItemType>>("itemTypes");
        itemCounts = getInputValue<std::unordered_map<std::string, int>>("itemCounts");
        itemWeights = getInputValue<std::unordered_map<std::string, float>>("itemWeights");
        itemValues = getInputValue<std::unordered_map<std::string, float>>("itemValues");
        itemSlots = getInputValue<std::vector<InventorySlot>>("itemSlots");
        itemFilters = getInputValue<std::vector<Filter>>("itemFilters");
        sorts = getInputValue<std::vector<Sort>>("sorts");

        // Compute results
        processInventory();

        setOutputValue("items", items);
        setOutputValue("itemTypes", itemTypes);
        setOutputValue("itemCounts", itemCounts);
        setOutputValue("itemWeights", itemWeights);
        setOutputValue("itemValues", itemValues);
        setOutputValue("itemSlots", itemSlots);
        setOutputValue("itemFilters", itemFilters);
        setOutputValue("sorts", sorts);
    }
    void addCategory(const std::string& category);
    void removeCategory(const std::string& category);
    void addItem(const Item& item);
    void removeItem(const Item& item);
    void processInventory();
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Inventory() = default;     // Default destructor
};

} // namespace hd
