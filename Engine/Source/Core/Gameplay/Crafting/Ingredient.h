/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Ingredient.h
 * @brief Header file for the Ingredient class.
 * 
 * ARCHITECTURAL NOTES:
 * - Ingredient is a class that represents an ingredient in Hydragon.
 * - It is used to represent any ingredient in the game world, such as herbs, minerals, etc.
 * - It supports interactive features, such as combining, and producing results. Also supports two-way messaging with other ingredients, environment, character, etc.
 */
#pragma once
#include "Core/NodeGraph/Node.h"
#include "Core/DataTable.h"
#include <string>

namespace hd {

struct IngredientInfo : public NodeInfo {
    IngredientInfo() {
        nodeType = "Gameplay/Ingredient";
        
        inputs = {
            "ingredientType",  // Type of ingredient (herb, mineral, etc.)
            "ingredientData",  // Ingredient data
            "environment",     // Environment data
            "characterData",   // Character data
            "ingredientState"  // Ingredient state
        };
        
        outputs = {
            "ingredientStatus",  // Ingredient status
            "ingredientMetrics"  // Ingredient performance metrics
        };
    }
};

class Ingredient : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Ingredient(const IngredientInfo& info = IngredientInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addIngredient(const std::string& ingredientName);
    void removeIngredient(const std::string& ingredientName);
    void combine();
    void produceResult();
    void processIngredient();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Ingredient() = default;     // Default destructor
};

} // namespace hd
