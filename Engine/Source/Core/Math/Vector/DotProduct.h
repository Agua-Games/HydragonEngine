/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DotProduct.h
 * @brief Header file for the DotProduct class.
 * 
 * ARCHITECTURAL NOTES:
 * - DotProduct is a class that represents a dot product of two vectors in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct DotProductInfo : public NodeInfo {
    DotProductInfo() {   
        NodeType = "Math/DotProduct";
        
        inputs = {
            "Vector1",  // First vector
            "Vector2"   // Second vector
        };
        
        outputs = {
            "DotProduct" // Dot product of the two vectors
        };
    }
};

class DotProduct : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DotProduct(const DotProductInfo& info = DotProductInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DotProductInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processDotProduct();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DotProduct() = default;     // Default destructor
};

} // namespace hd