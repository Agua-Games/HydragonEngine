/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Normalize.h
 * @brief Header file for the Normalize class.
 * 
 * ARCHITECTURAL NOTES:
 * - Normalize is a class that represents a normalization of a value to the range [0, 1] in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct NormalizeInfo : public NodeInfo {
    NormalizeInfo() {
        NodeType = "Logic/Normalize";
        
        inputs = {
            "InputValue",  // Input value to be normalized
            "NormalizeParams" // Additional parameters for the normalization
        };
        
        outputs = {
            "NormalizedValue" // Normalized output value
        };
    }
};

class Normalize : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Normalize(const NormalizeInfo& info = NormalizeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    NormalizeInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processNormalize();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Normalize() = default;     // Default destructor
};

} // namespace hd