/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIPanel.h
 * @brief Header file for the UIPanel class.
 * 
 * ARCHITECTURAL NOTES:
 * - UIPanel is a class that represents a panel in the user interface.
 * - It is used to group and organize UI elements in a specific layout.
 * - It supports various panel properties, such as size, position, and background color.
 * - It can be used to create various UI panels, such as inventory panels, toolbars, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct PanelInfo : public NodeInfo {
    PanelInfo() {
        NodeType = "Design/UIPanel";
        
        inputs = {
            "Size",          // Panel size
            "Position",      // Panel position
            "Background",    // Background layer
            "Elements"       // Array of UI elements
        };
        
        outputs = {
            "ComposedPanel", // Final composition
            "ElementBounds"  // Element positions
        };
    }
};

class UIPanel : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit UIPanel(const PanelInfo& info = PanelInfo())
        : Node(info), PanelInfo(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {
        // Process panel layout and positioning of UI elements
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~UIPanel() = default;     // Default destructor
};

} // namespace hd