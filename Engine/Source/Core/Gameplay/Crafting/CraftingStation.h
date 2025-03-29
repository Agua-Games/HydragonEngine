/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CraftingStation.h
 * @brief Header file for the CraftingStation class.
 * 
 * ARCHITECTURAL NOTES:
 * - CraftingStation is a class that represents a crafting station in Hydragon.
 * - It is used to represent any crafting station in the game world, such as blacksmiths, alchemist workshops, witch huts, etc.
 * - It supports interactive features, such as crafting, and producing results. Also supports two-way messaging with other crafting stations, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include "Node.h"
#include "CraftingManager.h"
#include "Recipe.h"
#include "CraftingResult.h"
#include "Item.h"
#include "MemoryBank.h"

namespace hd {

enum class CraftingStationType {
    blacksmith,
    alchemist,
    cook,
    witch,
    tailor,
    mechanic,
    engineer,
    custom
};

struct CraftingStationInfo : public NodeInfo {
    CraftingStationInfo() {
        nodeType = "Gameplay/CraftingStation";
        
        inputs = {
            "craftingStationType",  // Type of crafting station (blacksmith, alchemist, etc.)
            "craftingStationData",  // Crafting station data
            "environment",          // Environment data
            "characterData",        // Character data
            "craftingStationState"  // Crafting station state
        };
        
        outputs = {
            "craftingStationStatus",  // Crafting station status
            "craftingStationMetrics"  // Crafting station performance metrics
        };
    }
};

class CraftingStation : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CraftingStation(const CraftingStationInfo& info = CraftingStationInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    CraftingStationType type = CraftingStationType::custom;
    int level = 0;

    // === Processing ===
    void processNode() override {
 
    }
    void addCraftingStation(const std::string& craftingStationName);
    void removeCraftingStation(const std::string& craftingStationName);
    void queryCraftingStation(const std::string& craftingStationName);
    void combineIngredients();
    void processRecipe();
    void processCraftingResult();
    void addTool(const std::string& toolName);
    void removeTool(const std::string& toolName);
    void addIngredient(const std::string& ingredientName);
    void removeIngredient(const std::string& ingredientName);
    void useTool();
    void loadFromMemoryBank();
    void saveToMemoryBank();
    void craft();
    void produceResult();
    void processCraftingStation();
    void update();   // Update the crafting station state and data based on the input and output data.

    // === Cleanup ===
    void unload() override {}   // Unload the crafting station data.
    void cleanup() override {}  // Cleanup the crafting station data.
    ~CraftingStation() = default;     // Default destructor
};

} // namespace hd
