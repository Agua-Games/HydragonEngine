/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BeamSupport.h
 * @brief Header file for the BeamSupport class.
 * 
 * ARCHITECTURAL NOTES:
 * - BeamSupport is a class that represents a beam support in Hydragon.
 * - It is used to represent any beam support in the game world, such as building supports, bridge supports, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other beam supports, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct BeamSupportInfo : public BuildingElementInfo {
    BeamSupportInfo() {
        nodeType = "Gameplay/BeamSupport";
        
        inputs = {
            "beamSupportType",  // Type of beam support (building support, bridge support, etc.)
            "beamSupportData",  // Beam support data
            "environment",      // Environment data
            "characterData",    // Character data
            "beamSupportState"  // Beam support state
        };
        
        outputs = {
            "beamSupportStatus",  // Beam support status
            "beamSupportMetrics"  // Beam support performance metrics
        };
    }
};

class BeamSupport : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit BeamSupport(const BeamSupportInfo& info = BeamSupportInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
};

} // namespace hd
