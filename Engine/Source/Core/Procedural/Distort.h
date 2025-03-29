/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Distort.h
 * @brief Header file for the Distort class.
 * 
 * ARCHITECTURAL NOTES:
 * - Distort is a class that represents a distortion effect in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct DistortInfo : public NodeInfo {
    DistortInfo() {
        NodeType = "Procedural/Distort";
        
        inputs = {
            "InputData",  // Input data to be distorted
            "DistortType", // Type of distortion (e.g., noise, warp, etc.)
            "DistortParams"  // Additional parameters for the distortion
        };
        
        outputs = {
            "DistortedData" // Distorted output data
        };
    }
};

class Distort : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Distort(const DistortInfo& info = DistortInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DistortInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processDistort();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Distort() = default;     // Default destructor
};

} // namespace hd
