/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MontageManager.h
 * @brief Header file for the MontageManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - MontageManager is a class that represents a montage manager in Hydragon.
 * - It is used to manage, control, and modify montages.
 * - It uses the Vulkan API for montage processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Montage.h"

namespace hd {

struct MontageManagerInfo : public NodeInfo {
    MontageManagerInfo() {
        nodeType = "Animation/MontageManager";
        
        inputs = {
            "montages",        // Array of montages
            "montageState",    // Current state of the montage
            "montageSpeed",    // Speed of the montage
            "montageLoop"      // Whether the montage should loop
        };
        
        outputs = {
            "activeMontages",  // Active montages
            "montageMetrics"   // Performance and quality metrics
        };
    }
};

class MontageManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MontageManager(const MontageManagerInfo& info = MontageManagerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MontageManager() = default;     // Default destructor
};

} // namespace hd
