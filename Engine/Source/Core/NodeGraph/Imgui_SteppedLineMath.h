/** 
 * Hydragon original code is under the following license:
 * 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * The Imgui Stepped Line Math is a custom implementation for stepped line calculations.
 */
/**
 * imgui and imgui-node-editor code are under their respective licenses, found in
 * the ThirdParty directory (Source/ThirdParty/Licenses/).
 */
#ifndef __IMGUI_STEPPED_LINE_MATH_H__
#define __IMGUI_STEPPED_LINE_MATH_H__
#pragma once

#include "imgui_extra_math.h"
#include "imgui.h"

class ImSteppedLineRenderer {
public:
    static void ImSteppedLineRenderer::DrawLinkShapeHandle(
        ImVec2 ratioPoint, 
        ImDrawList* drawList, 
        float size, ImU32 
        color);

    static void DrawHydragonLine(
        ImDrawList* drawList,
        const ImVec2& start,
        const ImVec2& end,
        ImU32 color,
        float thickness,
        bool enableHandle);
    
    static void DrawSteppedLine(
        ImDrawList* drawList,
        const ImVec2& start,
        const ImVec2& end,
        ImU32 color,
        float thickness,
        bool enableHandle);
};

// Template version for modern C++ callbacks
template <typename F>
inline void ImSteppedLineFixedStep(
    F& callback,
    const ImVec2& start,
    const ImVec2& end,
    float step_position = 0.5f,
    bool horizontal_first = true
);

#endif // __IMGUI_STEPPED_LINE_MATH_H__
