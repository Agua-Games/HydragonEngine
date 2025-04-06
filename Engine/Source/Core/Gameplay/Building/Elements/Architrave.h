/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Architrave.h
 * @brief Header file for the Architrave class.
 * 
 * ARCHITECTURAL NOTES:
 * - Architrave is a class that represents an architrave in Hydragon.
 * - It is used to represent any architrave in the game world, such as building architraves, bridge architraves, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other architraves, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct ArchitraveInfo : public BuildingElementInfo {
    ArchitraveInfo() {
        nodeType = "Gameplay/Architrave";
        
        inputs = {
            "architraveType",  // Type of architrave (building architrave, bridge architrave, etc.)
            "architraveData",  // Architrave data
            "environment", // Environment data
            "characterData", // Character data
            "architraveState" // Architrave state
        };
        
        outputs = {
            "architraveStatus", // Architrave status
            "architraveMetrics" // Architrave performance metrics
        };
    }
};

class Architrave : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Architrave(const ArchitraveInfo& info = ArchitraveInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    
    }
    void update();
    
    void enter();
    void exit();
    void interact();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Architrave() = default;     // Default destructor
};

} // namespace hd
