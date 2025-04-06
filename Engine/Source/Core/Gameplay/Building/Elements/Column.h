/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Column.h
 * @brief Header file for the Column class.
 * 
 * ARCHITECTURAL NOTES:
 * - Column is a class that represents a column in Hydragon.
 * - It is used to represent any column in the game world, such as columns, supports, etc.
 * - It supports composition from different modular elements, like base, shaft, capit, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other columns, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct ColumnInfo : public BuildingElementInfo {
    ColumnInfo() {
        nodeType = "Gameplay/Column";
        
        inputs = {
            "columnType",  // Type of column (column, support, etc.)
            "columnData",  // column data
            "environment", // Environment data
            "characterData", // Character data
            "columnState" // column state
        };
        
        outputs = {
            "columnStatus", // column status
            "columnMetrics" // column performance metrics
        };
    }
};

class Column : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Column(const ColumnInfo& info = ColumnInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
    void interact();     // Interact with the column (e.g., open a door)
    void update();       // Update the column's state and status (e.g., check if the door is open)

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Column() = default;     // Default destructor
};

} // namespace hd
