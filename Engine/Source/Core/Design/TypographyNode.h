/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TypographyNode.h
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
        
        Inputs = {
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
        
        Outputs = {
            "TextImage",     // Rendered text
            "TextMetrics",   // Font metrics data
            "TextBounds"     // Bounding box
        };
    }
};

class TypographyNode : public Node<RenderTarget, FontMetrics, BoundingBox> {
public:
    explicit TypographyNode(const TypographyInfo& info = TypographyInfo())
        : Node(info), TypoInfo(info) {}

    void ProcessNodeGraph() override {
        auto text = GetInputValue<std::string>("Text");
        auto font = GetInputValue<FontAsset>("Font");
        float fontSize = GetInputValue<float>("FontSize");
        float lineHeight = GetInputValue<float>("LineHeight");
        float letterSpacing = GetInputValue<float>("LetterSpacing");
        TextAlignment alignment = GetInputValue<TextAlignment>("Alignment");
        glm::vec4 color = GetInputValue<glm::vec4>("Color");
        TextShadow shadow = GetInputValue<TextShadow>("Shadow");
        float maxWidth = GetInputValue<float>("MaxWidth");

        // Process typography
        auto textImage = RenderText(text, font, fontSize, lineHeight, 
                                  letterSpacing, alignment, color, 
                                  shadow, maxWidth);
        auto metrics = CalculateMetrics(text, font, fontSize);
        auto bounds = CalculateBounds(metrics, maxWidth);

        // Set outputs
        SetOutputValue("TextImage", textImage);
        SetOutputValue("TextMetrics", metrics);
        SetOutputValue("TextBounds", bounds);
    }

private:
    TypographyInfo TypoInfo;
};

} // namespace hd