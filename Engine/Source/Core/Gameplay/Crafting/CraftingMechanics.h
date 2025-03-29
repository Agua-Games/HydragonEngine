/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CraftingMechanics.h
 * @brief Header file for the CraftingMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - CraftingMechanics is a class that represents crafting mechanics in Hydragon.
 * - It is used to represent any crafting mechanics in the game world, such as recipes, ingredients, etc.
 * - It supports interactive features, such as crafting, and producing results. Also supports two-way messaging with other crafting mechanics, environment, character, etc.
 */
#pragma once

#include "Core.h"
#include "Node.h"
#include "CraftingSystem.h"
#include "CraftingStation.h"
#include "Recipe.h"
#include "CraftingResult.h"

namespace hd {

class CraftingMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CraftingMechanics(const CraftingMechanicsInfo& info = CraftingMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void validateItem();
    void validateRecipe();
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
    void processCraftingMechanics();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CraftingMechanics() = default;     // Default destructor
};

} // namespace hd

