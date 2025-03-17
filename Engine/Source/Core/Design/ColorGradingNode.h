/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ColorGradingNode.h
 * @brief Color grading node for image processing.
 * 
 * ARCHITECTURAL NOTES:
 * - Color grading nodes are used for adjusting image colors.
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
#include "ColorSpace.h"

namespace hd {

struct ColorGradingInfo : public NodeInfo {
    ColorGradingInfo() {
        NodeType = "Design/ColorGrading";
        
        Inputs = {
            "SourceImage",      // Input image
            "LUT",             // Optional color lookup table
            "Exposure",        // Exposure adjustment
            "Contrast",        // Contrast adjustment
            "Saturation",      // Saturation adjustment
            "Temperature",     // Color temperature
            "Tint",           // Green-Magenta tint
            "Vibrance",       // Intelligent saturation
            "ColorBalance"     // Shadows/Midtones/Highlights balance
        };
        
        Outputs = {
            "ProcessedImage",  // Color graded result
            "Histogram",      // Color distribution data
            "Waveform"        // Luminance distribution
        };
    }
};

class ColorGradingNode : public Node<RenderTarget, HistogramData, WaveformData> {
public:
    explicit ColorGradingNode(const ColorGradingInfo& info = ColorGradingInfo())
        : Node(info), GradingInfo(info) {}

    void processNodeGraph() override {
        auto source = getInputValue<RenderTarget>("SourceImage");
        auto lut = getInputValue<RenderTarget>("LUT");
        
        // Get adjustment values
        float exposure = getInputValue<float>("Exposure");
        float contrast = getInputValue<float>("Contrast");
        float saturation = getInputValue<float>("Saturation");
        float temperature = getInputValue<float>("Temperature");
        float tint = getInputValue<float>("Tint");
        float vibrance = getInputValue<float>("Vibrance");
        auto colorBalance = getInputValue<glm::vec3>("ColorBalance");

        // Process image
        auto result = processImage(source, lut, exposure, contrast, 
                                 saturation, temperature, tint, 
                                 vibrance, colorBalance);

        // Generate analysis data
        auto histogram = generateHistogram(result);
        auto waveform = generateWaveform(result);

        // Set outputs
        setOutputValue("ProcessedImage", result);
        setOutputValue("Histogram", histogram);
        setOutputValue("Waveform", waveform);
    }

    void drawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Color Grading");
        
        drawInputPort("SourceImage", "Source");
        drawInputPort("LUT", "LUT");
        
        // Draw sliders for adjustments
        float exposure = getInputValue<float>("Exposure");
        ImGui::SliderFloat("Exposure", &exposure, -5.0f, 5.0f);
        setInputValue("Exposure", exposure);
        
        drawOutputPort("ProcessedImage", "Result");
        drawOutputPort("Histogram", "Histogram");
        
        ImGui::EndGroup();
    }

private:
    ColorGradingInfo GradingInfo;
};

} // namespace hd