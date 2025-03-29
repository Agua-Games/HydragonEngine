/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Max.h
 * @brief Header file for the Max class.
 * 
 * ARCHITECTURAL NOTES:
 * - Max is a class that represents the maximum of two values in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct MaxInfo : public NodeInfo {
    MaxInfo() {
        NodeType = "Logic/Max";
        
        inputs = {
            "Value1",  // First value
            "Value2"   // Second value
        };
        
        outputs = {
            "MaxValue" // Maximum of the two values
        };
    }
};

class Max : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Max(const MaxInfo& info = MaxInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    MaxInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processMax();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Max() = default;     // Default destructor
};

} // namespace hd