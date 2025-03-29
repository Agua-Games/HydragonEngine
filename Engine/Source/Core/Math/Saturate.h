/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Saturate.h
 * @brief Header file for the Saturate class.
 * 
 * ARCHITECTURAL NOTES:
 * - Saturate is a class that represents a cheap normalization of a value to the range [0, 1] in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct SaturateInfo : public NodeInfo {
    SaturateInfo() {
        NodeType = "Logic/Saturate";
        
        inputs = {
            "InputValue",  // Input value to be saturated
            "SaturateParams" // Additional parameters for the saturation
        };
        
        outputs = {
            "SaturatedValue" // Saturated output value
        };
    }
};

class Saturate : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Saturate(const SaturateInfo& info = SaturateInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    SaturateInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processSaturate();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Saturate() = default;     // Default destructor
};

} // namespace hd