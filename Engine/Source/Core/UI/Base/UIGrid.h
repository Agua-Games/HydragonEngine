/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIGrid.h
 * @brief Header file for the UIGrid class.
 * 
 * ARCHITECTURAL NOTES:
 * - UIGrid is a class that represents a grid in the user interface.
 * - It is used to layout and position UI elements in a grid-like structure.
 * - It supports various grid properties, such as number of columns, spacing, and item size.
 * - It can be used to create various UI layouts, such as inventory grids, toolbars, etc.
 */
#pragma once
#include "Node.h"
#include "UIManager.h" // For vec2 type
//#include <glm/gtc/matrix_transform.hpp>

namespace hd {

struct GridInfo : public NodeInfo {
    GridInfo() {
        NodeType = "Design/UIGrid";
        
        inputs = {
            "Columns",       // Number of columns in the grid
            "Spacing",       // Spacing between grid items
            "ItemSize",      // Size of each grid item
            "Items",         // Array of UI elements to be placed in the grid
            "Alignment",     // Grid alignment
            "Distribution", // Content distribution
            "Background"    // Background layer
        };
        
        outputs = {
            "ComposedGrid",  // Final composition
            "ElementBounds", // Element positions
            "GridGuides"     // Grid visualization
        };
    }
};

class UIGrid : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit UIGrid(const GridInfo& info = GridInfo())
        : Node(info), GridInfo(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    int columns = 1;
    vec2 spacing = vec2(0.0f);
    vec2 itemSize = vec2(0.0f);
    
    // === Processing ===
    void processNode() override {
        // Process grid layout and positioning of UI elements
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~UIGrid() = default;     // Default destructor
};

} // namespace hd