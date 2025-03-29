/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BlurLogic.h
 * @brief Header file for the BlurLogic class.
 * 
 * ARCHITECTURAL NOTES:
 * - BlurLogic is a class that represents a blur logic node in Hydragon. The suffix "logic" (its category) is to avoid confusion with the Blur postprocess node.
 * - It uses the Vulkan API for blur logic processing.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct BlurLogicInfo : public NodeInfo {
    BlurLogicInfo() {
        NodeType = "Logic/BlurLogic";
        
        inputs = {
            "InputData",  // Input data to be blurred
            "BlurRadius", // Radius of the blur
            "BlurType",   // Type of blur (e.g., Gaussian, Box, etc.)
            "BlurParams"  // Additional parameters for the blur
        };
        
        outputs = {
            "BlurredData" // Blurred output data
        };
    }
};

class BlurLogic : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BlurLogic(const BlurLogicInfo& info = BlurLogicInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    BlurLogicInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processBlurLogic();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BlurLogic() = default;     // Default destructor
};

} // namespace hd