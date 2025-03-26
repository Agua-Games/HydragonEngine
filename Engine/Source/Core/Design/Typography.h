/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Typography.h
 * @brief Typography node for text rendering and layout.
 * 
 * ARCHITECTURAL NOTES:
 * - Typography nodes are used for rendering and styling text.
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
#include "FontSystem.h"
#include "RenderTarget.h"

namespace hd {

struct TypographyInfo : public NodeInfo {
    TypographyInfo() {
        nodeType = "Design/Typography";
        
        inputs = {
            "text",           // Input text content
            "font",           // Font asset
            "fontSize",       // Base font size
            "lineHeight",     // Line spacing
            "letterSpacing", // Character spacing
            "alignment",     // Text alignment
            "color",         // Text color
            "shadow",        // Text shadow
            "maxWidth"       // Text box width
        };
        
        outputs = {
            "textImage",     // Rendered text
            "textMetrics",   // Font metrics data
            "textBounds"     // Bounding box
        };
    }
};

class Typography : public Node<RenderTarget, FontMetrics, BoundingBox> {
public:
    // === Allocation, Initialization, Loading ===
    explicit Typography(const TypographyInfo& info = TypographyInfo())
        : Node(info), TypoInfo(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    std::string text;
    FontAsset* font = nullptr;
    float fontSize = 12.0f;
    float lineHeight = 1.0f;
    float letterSpacing = 0.0f;
    TextAlignment alignment = TextAlignment::Left;
    glm::vec4 color = glm::vec4(1.0f);
    TextShadow shadow;
    float maxWidth = 0.0f;

    // === Processing ===
    void processNode() override {
        text = getInputValue<std::string>("Text");
        font = getInputValue<FontAsset>("Font");
        fontSize = getInputValue<float>("FontSize");
        lineHeight = getInputValue<float>("LineHeight");
        letterSpacing = getInputValue<float>("LetterSpacing");
        alignment = getInputValue<TextAlignment>("Alignment");
        color = getInputValue<glm::vec4>("Color");
        shadow = getInputValue<TextShadow>("Shadow");
        maxWidth = getInputValue<float>("MaxWidth");

        // Process typography
        auto textImage = renderText(text, font, fontSize, lineHeight, 
                                  letterSpacing, alignment, color, 
                                  shadow, maxWidth);
        auto metrics = calculateMetrics(text, font, fontSize);
        auto bounds = calculateBounds(metrics, maxWidth);

        // Set outputs
        setOutputValue("TextImage", textImage);
        setOutputValue("TextMetrics", metrics);
        setOutputValue("TextBounds", bounds);
    }

private:
    TypographyInfo TypoInfo;
};

} // namespace hd