/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Tensor.h
 * @brief Header file for the Tensor class.
 * 
 * ARCHITECTURAL NOTES:
 * - Tensor is a class that represents a tensor in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct TensorInfo : public NodeInfo {
    TensorInfo() {
        NodeType = "Math/Tensor";
        
        inputs = {
            "Dimensions",  // Dimensions of the tensor
            "Data"        // Data to be stored in the tensor
        };
        
        outputs = {
            "Tensor" // Tensor data
        };
    }
};

class Tensor : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Tensor(const TensorInfo& info = TensorInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    TensorInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processTensor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Tensor() = default;     // Default destructor
};

} // namespace hd