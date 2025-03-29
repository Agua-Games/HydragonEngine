/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ColorGrading.h
 * @brief Color grading node for image processing.
 * 
 * ARCHITECTURAL NOTES:
 * - Color grading nodes are used for adjusting image colors.
 * - For now we're keeping a separate file for design color gradient and rendering postprocess color gradient, because of possible differences in implementation,
 * as the postprocess effect must comply with Vulkan requirements, setup, etc - but if possible we're going to integrate them into a single node later, versatile.
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
        nodeType = "Design/ColorGrading";
        
        inputs = {
            "sourceImage",      // Input image
            "LUT",             // Optional color lookup table
            "exposure",        // Exposure adjustment
            "contrast",        // Contrast adjustment
            "saturation",      // Saturation adjustment
            "temperature",     // Color temperature
            "tint",           // Green-Magenta tint
            "vibrance",       // Intelligent saturation
            "colorBalance"     // Shadows/Midtones/Highlights balance
        };
        
        outputs = {
            "processedImage",  // Color graded result
            "histogram",      // Color distribution data
            "waveform"        // Luminance distribution
        };
    }
};

class ColorGrading : public Node<RenderTarget, HistogramData, WaveformData> {
public:
    // === Allocation, Initialization, Loading ===
    explicit ColorGrading(const ColorGradingInfo& info = ColorGradingInfo())
        : Node(info), GradingInfo(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    RenderTarget* source = nullptr;
    RenderTarget* lut = nullptr;
    float exposure = 0.0f;
    float contrast = 0.0f;
    float saturation = 0.0f;
    float temperature = 0.0f;
    float tint = 0.0f;
    float vibrance = 0.0f;
    glm::vec3 colorBalance = glm::vec3(0.0f);

    // === Processing ===
    void processNode() override {
        source = getInputValue<RenderTarget>("sourceImage");
        lut = getInputValue<RenderTarget>("LUT");
        
        // Get adjustment values
        exposure =      getInputValue<float>("exposure");
        contrast =      getInputValue<float>("contrast");
        saturation =    getInputValue<float>("saturation");
        temperature =   getInputValue<float>("temperature");
        tint =          getInputValue<float>("tint");
        vibrance =      getInputValue<float>("vibrance");
        colorBalance =  getInputValue<glm::vec3>("colorBalance");

        // Process image
        auto result = processImage(source, lut, exposure, contrast, 
                                 saturation, temperature, tint, 
                                 vibrance, colorBalance);

        // Generate analysis data
        auto histogram = generateHistogram(result);
        auto waveform = generateWaveform(result);

        // Set outputs
        setOutputValue("processedImage", result);
        setOutputValue("histogram", histogram);
        setOutputValue("waveform", waveform);
    }

    void drawInNodeGraph() override {
        
    }
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ColorGrading() = default;     // Default destructor

private:
    ColorGradingInfo GradingInfo;
};

} // namespace hd