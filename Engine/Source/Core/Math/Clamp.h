/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Clamp.h
 * @brief Header file for the Clamp class.
 * 
 * ARCHITECTURAL NOTES:
 * - Clamp is a class that represents a clamping of a value to a range in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct ClampInfo : public NodeInfo {
    ClampInfo() {
        NodeType = "Logic/Clamp";
        
        inputs = {
            "InputValue",  // Input value to be clamped
            "ClampParams" // Additional parameters for the clamping
        };
        
        outputs = {
            "ClampedValue" // Clamped output value
        };
    }
};

class Clamp : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Clamp(const ClampInfo& info = ClampInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ClampInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processClamp();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Clamp() = default;     // Default destructor
};

} // namespace hd