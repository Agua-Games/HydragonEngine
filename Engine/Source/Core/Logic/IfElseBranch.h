/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file IfElseBranch.h
 * @brief Header file for the IfElseBranch class.
 * 
 * ARCHITECTURAL NOTES:
 * - IfElseBranch is a class that represents an if-else branch in Hydragon.
 * - It is used to represent and process if-else branches.
 * - It uses the Vulkan API for if-else branch processing.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct IfElseBranchInfo : public NodeInfo {
    enum class Operator {
        Equal,
        NotEqual,
        Greater,
        Less,
        GreaterEqual,
        LessEqual,
        Custom
    };

    IfElseBranchInfo() {
        NodeType = "Logic/IfElseBranch";
        
        inputs = {
            "FirstTerm",  // First term of the if-else branch
            "Operator",   // Operator of the if-else branch
            "SecondTerm", // Second term of the if-else branch
            "TrueBranch", // True branch
            "FalseBranch" // False branch
        };
        
        outputs = {
            "Result" // Result of the if-else branch
        };
    }
};

class IfElseBranch : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit IfElseBranch(const IfElseBranchInfo& info = IfElseBranchInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    IfElseBranchInfo info;
    IfElseBranchInfo.Operator op = IfElseBranchInfo.Operator::Equal;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
 
    }
    void processIfElseBranch();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~IfElseBranch() = default;     // Default destructor
};

} // namespace hd