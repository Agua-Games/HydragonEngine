/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Recipe.h
 * @brief Header file for the Recipe class.
 * 
 * ARCHITECTURAL NOTES:
 * - Recipe is a class that represents a recipe in Hydragon.
 * - It is used to represent any recipe in the game world, such as crafting, cooking, etc.
 * - It supports interactive features, such as combining ingredients, and producing results. Also supports two-way messaging with other recipes, environment, character, etc.
 */
#pragma once
#include "Core/NodeGraph/Node.h"
#include "Core/DataTable.h"
#include <string>

namespace hd {

struct RecipeInfo : public NodeInfo {
    RecipeInfo() {
        nodeType = "Gameplay/Recipe";
        
        inputs = {
            "recipeType",       // Type of recipe (crafting, cooking, etc.)
            "recipeData",       // Recipe data
            "environment",      // Environment data
            "characterData",    // Character data
            "recipeState"       // Recipe state
        };
        
        outputs = {
            "recipeStatus",     // Recipe status
            "recipeMetrics"     // Recipe performance metrics
        };
    }
};

class Recipe : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Recipe(const RecipeInfo& info = RecipeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addRecipe(const std::string& recipeName);
    void removeRecipe(const std::string& recipeName);
    void combineIngredients();
    void produceResult();
    void processRecipe();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Recipe() = default;     // Default destructor
};

} // namespace hd
