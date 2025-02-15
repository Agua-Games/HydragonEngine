/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Rendering/HD_RenderTarget.h"
#include "Core/Color/HD_ColorSpace.h"

namespace hd {

struct HD_ColorGradingInfo : public HD_NodeInfo {
    HD_ColorGradingInfo() {
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

class HD_ColorGradingNode : public HD_Node<RenderTarget, HistogramData, WaveformData> {
public:
    explicit HD_ColorGradingNode(const HD_ColorGradingInfo& info = HD_ColorGradingInfo())
        : HD_Node(info), GradingInfo(info) {}

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
    HD_ColorGradingInfo GradingInfo;
};

} // namespace hd