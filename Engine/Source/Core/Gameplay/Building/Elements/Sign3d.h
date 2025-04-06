/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Sign3d.h
 * @brief Header file for the Sign3d class.
 * 
 * ARCHITECTURAL NOTES:
 * - Sign3d is a class that represents a 3D sign in Hydragon.
 * - It is used to represent any 3D sign in the game world, such as building signs, bridge signs, etc.
 * - It supports 3d signs with emissive lighting, like neon signs.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other signs, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"
#include "LightMesh.h"

namespace hd {

struct Sign3dInfo : public BuildingElementInfo {
    Sign3dInfo() {
        nodeType = "Gameplay/Sign3d";
        
        inputs = {
            "sign3dType",  // Type of sign3d (building sign, bridge sign, etc.)
            "sign3dData",  // Sign3d data
            "environment", // Environment data
            "characterData", // Character data
            "sign3dState" // Sign3d state
        };
        
        outputs = {
            "sign3dStatus", // Sign3d status
            "sign3dMetrics" // Sign3d performance metrics
        };
    }
};

class Sign3d : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Sign3d(const Sign3dInfo& info = Sign3dInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    LightMesh lightMesh;
    bool isEmissive = false;
    float emissiveIntensity = 1.0f;

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
};

} // namespace hd
