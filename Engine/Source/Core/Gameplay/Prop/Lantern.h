/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Lantern.h
 * @brief Header file for the Lantern class.
 * 
 * ARCHITECTURAL NOTES:
 * - Lantern is a class that represents a battery powered lantern in Hydragon.
 * - It is used to represent any battery powered lantern in the game world, such as flashlights, etc.
 * - It supports interactive features, such as lighting, dimming, and flickering. Also supports two-way messaging with other lanterns, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Tool.h"
#include "DataTable.h"
#include "Lighting.h"

namespace hd {

struct LanternInfo : public ToolInfo {
    LanternInfo() {
        nodeType = "Gameplay/Lantern";
        
        inputs = {
            "lanternType",      // Type of lantern (flashlight, torch, etc.)
            "lanternData",      // Lantern data
            "environment",      // Environment data
            "characterData",    // Character data
            "lanternState"      // Lantern state
        };
        
        outputs = {
            "lanternStatus",    // Lantern status
            "lanternMetrics"    // Lantern performance metrics
        };
    }
};

class Lantern : public Tool {
public:
    // === Allocation, Initialization, Loading ===
    explicit Lantern(const LanternInfo& info = LanternInfo())
        : Tool(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processLantern();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Lantern() = default;     // Default destructor
};

} // namespace hd
