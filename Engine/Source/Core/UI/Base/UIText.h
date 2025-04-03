/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIText.h
 * @brief Header file for the UIText class.
 * 
 * ARCHITECTURAL NOTES:
 * - UIText is a class that represents a text element in the user interface.
 * - It is used to display text on the screen.
 * - It supports various text properties, such as font, size, color, alignment, etc.
 * - It can be used to create various UI text elements, such as labels, buttons, etc.
 */
#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include "Node.h"

namespace hd {

struct TextInfo : public NodeInfo {
    TextInfo() {
        NodeType = "Design/UIText";
        
        inputs = {
            "Text",        // Text content
            "Font",        // Font asset
            "FontSize",    // Base font size
            "LineHeight",  // Line spacing
            "LetterSpacing", // Character spacing
            "Alignment",   // Text alignment
            "Color",       // Text color
            "Shadow",      // Text shadow
            "MaxWidth"     // Text box width
        };
        
        outputs = {
            "TextImage",   // Rendered text
            "TextMetrics", // Font metrics data
            "TextBounds"   // Bounding box
        };
    }
};

class UIText : public Node {
public:
    // === Structure Definitons ===
    enum class TextAlignment {
        Left,
        Center,
        Right
    };

    struct TextShadow {
        glm::vec2 offset;
        glm::vec4 color;
    };

    // === Allocation, Initialization, Loading ===
    explicit UIText(const TextInfo& info = TextInfo())
        : Node(info) {}
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
        // Process inputs, compute results and prepare outputs
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~UIText() = default;     // Default destructor
private:
    TextInfo TextInfo;
};

} // namespace hd