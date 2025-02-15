/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Text/HD_FontSystem.h"
#include "Core/Rendering/HD_RenderTarget.h"

namespace hd {

struct HD_TypographyInfo : public HD_NodeInfo {
    HD_TypographyInfo() {
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

class HD_TypographyNode : public HD_Node<RenderTarget, FontMetrics, BoundingBox> {
public:
    explicit HD_TypographyNode(const HD_TypographyInfo& info = HD_TypographyInfo())
        : HD_Node(info), TypoInfo(info) {}

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
    HD_TypographyInfo TypoInfo;
};

} // namespace hd