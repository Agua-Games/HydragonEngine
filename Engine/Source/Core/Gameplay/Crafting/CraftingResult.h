/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CraftingResult.h
 * @brief Header file for the CraftingResult class.
 * 
 * ARCHITECTURAL NOTES:
 * - CraftingResult is a class that represents a crafting result in Hydragon.
 * - It is used to represent any crafting result in the game world, such as items, weapons, etc.
 * - It supports interactive features, such as equipping, and using. Also supports two-way messaging with other crafting results, environment, character, etc.
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "CraftingManager.h"
#include "CraftingStation.h"
#include "Item.h"

namespace hd {

struct CraftingResultInfo : public NodeInfo {
    CraftingResultInfo() {
        nodeType = "Gameplay/CraftingResult";
        
        inputs = {
            "craftingResultType",  // Type of crafting result (item, weapon, etc.)
            "craftingResultData",  // Crafting result data
            "environment",         // Environment data
            "characterData",       // Character data
            "craftingResultState"  // Crafting result state
        };
        
        outputs = {
            "craftingResultStatus",  // Crafting result status
            "craftingResultMetrics"  // Crafting result performance metrics
        };
    }
};

class CraftingResult : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CraftingResult(const CraftingResultInfo& info = CraftingResultInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    Item item;
    int quantity = 1;
    std::unordered_map<std::string, int> craftingResultData;

    // === Processing ===
    void processNode() override {
 
    }
    void addCraftingResult(const std::string& craftingResultName);
    void removeCraftingResult(const std::string& craftingResultName);
    void equip();
    void use();
    void processCraftingResult();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CraftingResult() = default;     // Default destructor
};

} // namespace hd
