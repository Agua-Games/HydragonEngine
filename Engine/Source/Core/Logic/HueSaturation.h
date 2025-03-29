/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HueSaturation.h
 * @brief Header file for the HueSaturation class.
 * 
 * ARCHITECTURAL NOTES:
 * - HueSaturation is a class that represents a hue and saturation adjustment node in Hydragon.
 * - It uses the Vulkan API for hue and saturation adjustment processing.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct HueSaturationInfo : public NodeInfo {
    HueSaturationInfo() {
        NodeType = "Logic/HueSaturation";
        
        inputs = {
            "InputData",  // Input data to be adjusted
            "Hue",        // Hue adjustment
            "Saturation", // Saturation adjustment
            "AdjustParams" // Additional parameters for the adjustment
        };
        
        outputs = {
            "AdjustedData" // Adjusted output data
        };
    }
};

class HueSaturation : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit HueSaturation(const HueSaturationInfo& info = HueSaturationInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    HueSaturationInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processHueSaturation();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~HueSaturation() = default;     // Default destructor
};

} // namespace hd