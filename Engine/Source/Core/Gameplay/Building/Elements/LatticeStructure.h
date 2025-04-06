/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LatticeStructure.h
 * @brief Header file for the LatticeStructure class.
 * 
 * ARCHITECTURAL NOTES:
 * - LatticeStructure is a class that represents a lattice structure in Hydragon.
 * - It is used to represent any lattice structure in the game world, such as building lattices, bridge lattices, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other lattice structures, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {
    
struct LatticeStructureInfo : public BuildingElementInfo {
    LatticeStructureInfo() {
        nodeType = "Gameplay/LatticeStructure";
        
        inputs = {
            "latticeStructureType",  // Type of lattice structure (building lattice, bridge lattice, etc.)
            "latticeStructureData",  // Lattice structure data
            "environment",           // Environment data
            "characterData",         // Character data
            "latticeStructureState"  // Lattice structure state
        };
        
        outputs = {
            "latticeStructureStatus",  // Lattice structure status
            "latticeStructureMetrics"  // Lattice structure performance metrics
        };
    }
};

class LatticeStructure : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit LatticeStructure(const LatticeStructureInfo& info = LatticeStructureInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
    void interact();     // Interact with the lattice structure (e.g., climb up, climb down, etc.)
};

} // namespace hd
