/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CrossProduct.h
 * @brief Header file for the CrossProduct class.
 * 
 * ARCHITECTURAL NOTES:
 * - CrossProduct is a class that represents a cross product of two vectors in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct CrossProductInfo : public NodeInfo {
    CrossProductInfo() {
        NodeType = "Math/CrossProduct";
        
        inputs = {
            "Vector1",  // First vector
            "Vector2"   // Second vector
        };
        
        outputs = {
            "CrossProduct" // Cross product of the two vectors
        };
    }
};

class CrossProduct : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CrossProduct(const CrossProductInfo& info = CrossProductInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    CrossProductInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processCrossProduct();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CrossProduct() = default;     // Default destructor
};

} // namespace hd