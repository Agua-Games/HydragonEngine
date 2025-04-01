/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Torch.h
 * @brief Header file for the Torch class.
 * 
 * ARCHITECTURAL NOTES:
 * - Torch is a class that represents a torch in Hydragon.
 * - It is used to represent any torch in the game world.
 * - It supports interactive features, such as lighting, dimming, and flickering. Also supports two-way messaging with other torches, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Prop.h"
#include "DataTable.h"
#include "Light.h"

namespace hd {

struct TorchInfo : public PropInfo {
    TorchInfo() {
        nodeType = "Gameplay/Torch";
        
        inputs = {
            "torchType",      // Type of torch (lantern, candle, etc.)
            "torchData",      // Torch data
            "environment",    // Environment data
            "characterData",  // Character data
            "torchState"      // Torch state
        };
        
        outputs = {
            "torchStatus",    // Torch status
            "torchMetrics"    // Torch performance metrics
        };
    }
};

class Torch : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Torch(const TorchInfo& info = TorchInfo())
        : Prop(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processTorch();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Torch() = default;     // Default destructor
};

} // namespace hd
