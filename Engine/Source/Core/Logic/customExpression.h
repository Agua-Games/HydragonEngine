/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief Header file for the CustomExpression class.
 * 
 * ARCHITECTURAL NOTES:
 * - CustomExpression is a class that represents a custom expression in Hydragon.
 * - It is used to evaluate custom expressions in the engine's node graph.
 * - It supports arbitrary expressions, such as arithmetic, logical, and relational operations.
 * - It supports custom functions and variables.
 * - It supports custom data types, such as vectors, matrices, and quaternions.
 * - It supports custom operators, such as dot product, cross product, and matrix multiplication.
 * - It supports custom evaluation modes, such as immediate and deferred evaluation.
 * - It supports custom error handling and reporting.
 * - It supports custom optimization and caching.
 * - It supports custom serialization and deserialization.
 * - It supports custom debugging and profiling.
 * - It supports custom visualization and inspection.
 * - It supports custom integration with other nodes and systems.
 * - It supports custom extensibility and customization.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Node.h"

namespace hd {

struct CustomExpressionInfo : public NodeInfo {
    CustomExpressionInfo() {
        NodeType = "Logic/CustomExpression";
        
        inputs = {
            "Expression",  // Custom expression to evaluate
            "Variables",   // Custom variables to use in the expression
            "Functions",   // Custom functions to use in the expression
            "DataTypes",   // Custom data types to use in the expression
            "Operators",   // Custom operators to use in the expression
            "EvaluationMode" // Custom evaluation mode to use
        };
        
        outputs = {
            "Result" // Result of the custom expression
        };
    }
};

class CustomExpression : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit CustomExpression(const CustomExpressionInfo& info = CustomExpressionInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CustomExpression() = default;     // Default destructor
};

} // namespace hd
