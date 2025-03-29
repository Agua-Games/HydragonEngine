/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Min.h
 * @brief Header file for the Min class.
 * 
 * ARCHITECTURAL NOTES:
 * - Min is a class that represents the minimum of two values in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct MinInfo : public NodeInfo {
    MinInfo() {
        NodeType = "Logic/Min";
        
        inputs = {
            "Value1",  // First value
            "Value2"   // Second value
        };
        
        outputs = {
            "MinValue" // Minimum of the two values
        };
    }
};

class Min : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Min(const MinInfo& info = MinInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    MinInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processMin();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Min() = default;     // Default destructor
};

} // namespace hd