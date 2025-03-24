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
        NodeType = "Design/Typography";
        
        inputs = {
            "Text",           // Input text content
            "Font",           // Font asset
            "FontSize",       // Base font size
            "LineHeight",     // Line spacing
            "LetterSpacing", // Character spacing
            "Alignment",     // Text alignment
            "Color",         // Text color
            "Shadow",        // Text shadow
            "MaxWidth"       // Text box width
        };
        
        outputs = {
            "TextImage",     // Rendered text
            "TextMetrics",   // Font metrics data
            "TextBounds"     // Bounding box
        };
    }
};

class Typography : public Node<RenderTarget, FontMetrics, BoundingBox> {
public:
    explicit Typography(const TypographyInfo& info = TypographyInfo())
        : Node(info), TypoInfo(info) {}

    void processNodeGraph() override {
        auto text = getInputValue<std::string>("Text");
        auto font = getInputValue<FontAsset>("Font");
        float fontSize = getInputValue<float>("FontSize");
        float lineHeight = getInputValue<float>("LineHeight");
        float letterSpacing = getInputValue<float>("LetterSpacing");
        TextAlignment alignment = getInputValue<TextAlignment>("Alignment");
        glm::vec4 color = getInputValue<glm::vec4>("Color");
        TextShadow shadow = getInputValue<TextShadow>("Shadow");
        float maxWidth = getInputValue<float>("MaxWidth");

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