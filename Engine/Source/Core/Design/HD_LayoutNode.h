/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Rendering/HD_RenderTarget.h"
#include "Core/Design/HD_LayoutSystem.h"

namespace hd {

struct HD_LayoutInfo : public HD_NodeInfo {
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

class HD_LayoutNode : public HD_Node<RenderTarget, std::vector<BoundingBox>, GridGuides> {
public:
    explicit HD_LayoutNode(const HD_LayoutInfo& info = HD_LayoutInfo())
        : HD_Node(info), LayoutInfo(info) {}

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