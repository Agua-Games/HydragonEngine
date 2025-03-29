/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BrightnessContrast.h
 * @brief Header file for the BrightnessContrast class.
 * 
 * ARCHITECTURAL NOTES:
 * - BrightnessContrast is a class that represents a brightness and contrast adjustment node in Hydragon.
 * - It uses the Vulkan API for brightness and contrast adjustment processing.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct BrightnessContrastInfo : public NodeInfo {
    BrightnessContrastInfo() {
        NodeType = "Logic/BrightnessContrast";
        
        inputs = {
            "InputData",  // Input data to be adjusted
            "Brightness", // Brightness adjustment
            "Contrast",   // Contrast adjustment
            "AdjustParams" // Additional parameters for the adjustment
        };
        
        outputs = {
            "AdjustedData" // Adjusted output data
        };
    }
};

class BrightnessContrast : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BrightnessContrast(const BrightnessContrastInfo& info = BrightnessContrastInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    BrightnessContrastInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processBrightnessContrast();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BrightnessContrast() = default;     // Default destructor
};

} // namespace hd