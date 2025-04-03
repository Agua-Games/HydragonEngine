/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIButton.h
 * @brief Header file for the UIButton class.
 * 
 * ARCHITECTURAL NOTES:
 * - UIButton is a class that represents a button element in the user interface.
 * - It is used to display buttons on the screen.
 * - It supports various button properties, such as size, position, color, text, etc.
 * - It can be used to create various UI buttons, such as action buttons, menu buttons, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct ButtonInfo : public NodeInfo {
    ButtonInfo() {
        NodeType = "Design/UIButton";
        
        inputs = {
            "Size",          // Button size
            "Position",      // Button position
            "Color",         // Button color
            "Text",          // Button text
            "Font",          // Button font
            "FontSize",      // Button font size
            "Alignment",     // Button text alignment
            "Shadow",        // Button text shadow
            "MaxWidth"       // Button text box width
        };
        
        outputs = {
            "ButtonImage",   // Rendered button
            "ButtonMetrics", // Button metrics data
            "ButtonBounds"   // Button bounding box
        };
    }
};

class UIButton : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit UIButton(const ButtonInfo& info = ButtonInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {
        // Process button rendering and metrics
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~UIButton() = default;     // Default destructor
private:
    ButtonInfo ButtonInfo;
};

} // namespace hd