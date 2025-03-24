/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LayoutUI.h
 * @brief LayoutUI node for UI composition.
 * 
 * ARCHITECTURAL NOTES:
 * - LayoutUI nodes are used for composing UI elements.
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
#include "RenderTarget.h"
#include "LayoutSystem.h"

namespace hd {

struct LayoutInfo : public NodeInfo {
    LayoutInfo() {
        NodeType = "Design/LayoutUI";
        
        Inputs = {
            "Elements",      // Array of layout elements
            "Container",     // Container constraints
            "GridSystem",    // Grid configuration
            "Spacing",       // Element spacing
            "Alignment",     // LayoutUI alignment
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

class LayoutUI : public Node<RenderTarget, std::vector<BoundingBox>, GridGuides> {
public:
    explicit LayoutUI(const LayoutInfo& info = LayoutInfo())
        : Node(info), LayoutInfo(info) {}

    void processNodeGraph() override {
        auto elements = getInputValue<std::vector<LayoutElement>>("Elements");
        auto container = getInputValue<ContainerConstraints>("Container");
        auto grid = getInputValue<GridSystem>("GridSystem");
        float spacing = getInputValue<float>("Spacing");
        LayoutAlignment alignment = getInputValue<LayoutAlignment>("Alignment");
        Distribution distribution = getInputValue<Distribution>("Distribution");
        auto responsive = getInputValue<ResponsiveConfig>("Responsive");
        auto background = getInputValue<RenderTarget>("Background");

        // Process layout
        auto composition = composeLayout(elements, container, grid, 
                                       spacing, alignment, distribution, 
                                       responsive, background);
        auto bounds = calculateElementBounds(elements, grid);
        auto guides = generateGridGuides(grid, container);

        // Set outputs
        setOutputValue("ComposedLayout", composition);
        setOutputValue("ElementBounds", bounds);
        setOutputValue("GridGuides", guides);
    }

private:
    LayoutInfo LayoutInfo;
};

} // namespace hd