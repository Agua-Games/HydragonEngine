/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_LayoutNode.h
 * @brief Layout node for UI composition.
 * 
 * ARCHITECTURAL NOTES:
 * - Layout nodes are used for composing UI elements.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once
#include "Node.h"
#include "HD_RenderTarget.h"
#include "HD_LayoutSystem.h"

namespace hd {

struct HD_LayoutInfo : public NodeInfo {
    HD_LayoutInfo() {
        NodeType = "Design/Layout";
        
        Inputs = {
            "Elements",      // Array of layout elements
            "Container",     // Container constraints
            "GridSystem",    // Grid configuration
            "Spacing",       // Element spacing
            "Alignment",     // Layout alignment
            "Distribution", // Content distribution
            "Responsive",   // Responsive breakpoints
            "Background"    // Background layer
        };
        
        Outputs = {
            "ComposedLayout",  // Final composition
            "ElementBounds",   // Element positions
            "GridGuides"       // Grid visualization
        };
    }
};

class HD_LayoutNode : public Node<RenderTarget, std::vector<BoundingBox>, GridGuides> {
public:
    explicit HD_LayoutNode(const HD_LayoutInfo& info = HD_LayoutInfo())
        : Node(info), LayoutInfo(info) {}

    void ProcessNodeGraph() override {
        auto elements = GetInputValue<std::vector<LayoutElement>>("Elements");
        auto container = GetInputValue<ContainerConstraints>("Container");
        auto grid = GetInputValue<GridSystem>("GridSystem");
        float spacing = GetInputValue<float>("Spacing");
        LayoutAlignment alignment = GetInputValue<LayoutAlignment>("Alignment");
        Distribution distribution = GetInputValue<Distribution>("Distribution");
        auto responsive = GetInputValue<ResponsiveConfig>("Responsive");
        auto background = GetInputValue<RenderTarget>("Background");

        // Process layout
        auto composition = ComposeLayout(elements, container, grid, 
                                       spacing, alignment, distribution, 
                                       responsive, background);
        auto bounds = CalculateElementBounds(elements, grid);
        auto guides = GenerateGridGuides(grid, container);

        // Set outputs
        SetOutputValue("ComposedLayout", composition);
        SetOutputValue("ElementBounds", bounds);
        SetOutputValue("GridGuides", guides);
    }

private:
    HD_LayoutInfo LayoutInfo;
};

} // namespace hd