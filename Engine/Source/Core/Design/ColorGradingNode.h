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

    void ProcessNodeGraph() override {
        auto source = GetInputValue<RenderTarget>("SourceImage");
        auto lut = GetInputValue<RenderTarget>("LUT");
        
        // Get adjustment values
        float exposure = GetInputValue<float>("Exposure");
        float contrast = GetInputValue<float>("Contrast");
        float saturation = GetInputValue<float>("Saturation");
        float temperature = GetInputValue<float>("Temperature");
        float tint = GetInputValue<float>("Tint");
        float vibrance = GetInputValue<float>("Vibrance");
        auto colorBalance = GetInputValue<glm::vec3>("ColorBalance");

        // Process image
        auto result = ProcessImage(source, lut, exposure, contrast, 
                                 saturation, temperature, tint, 
                                 vibrance, colorBalance);

        // Generate analysis data
        auto histogram = GenerateHistogram(result);
        auto waveform = GenerateWaveform(result);

        // Set outputs
        SetOutputValue("ProcessedImage", result);
        SetOutputValue("Histogram", histogram);
        SetOutputValue("Waveform", waveform);
    }

    void DrawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Color Grading");
        
        DrawInputPort("SourceImage", "Source");
        DrawInputPort("LUT", "LUT");
        
        // Draw sliders for adjustments
        float exposure = GetInputValue<float>("Exposure");
        ImGui::SliderFloat("Exposure", &exposure, -5.0f, 5.0f);
        SetInputValue("Exposure", exposure);
        
        DrawOutputPort("ProcessedImage", "Result");
        DrawOutputPort("Histogram", "Histogram");
        
        ImGui::EndGroup();
    }

private:
    ColorGradingInfo GradingInfo;
};

} // namespace hd