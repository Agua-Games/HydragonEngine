/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UISlider.h
 * @brief Header file for the UISlider class.
 * 
 * ARCHITECTURAL NOTES:
 * - UISlider is a class that represents a slider element in the user interface.
 * - It is used to display sliders on the screen.
 * - It supports various slider properties, such as size, position, color, value, etc.
 * - It can be used to create various UI sliders, such as volume sliders, brightness sliders, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct SliderInfo : public NodeInfo {
    SliderInfo() {
        NodeType = "Design/UISlider";
        
        inputs = {
            "Size",          // Slider size
            "Position",      // Slider position
            "Color",         // Slider color
            "Value",         // Slider value
            "MinValue",      // Minimum value
            "MaxValue",      // Maximum value
            "Step",          // Step size
            "Orientation",   // Slider orientation
            "Background"     // Background layer
        };
        
        outputs = {
            "SliderImage",   // Rendered slider
            "SliderMetrics", // Slider metrics data
            "SliderBounds"   // Slider bounding box
        };
    }
};

class UISlider : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit UISlider(const SliderInfo& info = SliderInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {
        // Process slider rendering and metrics
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~UISlider() = default;     // Default destructor
private:
    SliderInfo SliderInfo;
};

} // namespace hd