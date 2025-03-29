/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Magnitude.h
 * @brief Header file for the Magnitude class.
 * 
 * ARCHITECTURAL NOTES:
 * - Magnitude is a class that represents the magnitude of a vector in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct MagnitudeInfo : public NodeInfo {
    MagnitudeInfo() {
        NodeType = "Math/Magnitude";
        
        inputs = {
            "Vector" // Input vector
        };
        
        outputs = {
            "Magnitude" // Magnitude of the vector
        };
    }
};

class Magnitude : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Magnitude(const MagnitudeInfo& info = MagnitudeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    MagnitudeInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processMagnitude();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Magnitude() = default;     // Default destructor
};

} // namespace hd