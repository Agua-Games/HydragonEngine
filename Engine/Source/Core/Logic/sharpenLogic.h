/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SharpenLogic.h
 * @brief Header file for the SharpenLogic class.
 * 
 * ARCHITECTURAL NOTES:
 * - SharpenLogic is a class that represents a sharpen logic node in Hydragon. The suffix "logic" (its category) is to avoid confusion with the Sharpen postprocess node.
 * - It uses the Vulkan API for sharpen logic processing.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct SharpenLogicInfo : public NodeInfo {
    SharpenLogicInfo() {
        NodeType = "Logic/SharpenLogic";
        
        inputs = {
            "InputData",  // Input data to be sharpened
            "SharpenAmount", // Amount of sharpening
            "SharpenType",   // Type of sharpening (e.g., Unsharp Mask, High Pass, etc.)
            "SharpenParams"  // Additional parameters for the sharpening
        };
        
        outputs = {
            "SharpenedData" // Sharpened output data
        };
    }
};

class SharpenLogic : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SharpenLogic(const SharpenLogicInfo& info = SharpenLogicInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    SharpenLogicInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processSharpenLogic();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SharpenLogic() = default;     // Default destructor
};

} // namespace hd