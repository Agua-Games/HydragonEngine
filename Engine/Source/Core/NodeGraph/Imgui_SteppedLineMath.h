/** 
 * Hydragon original code is under the following license:
 * 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * The Node Graph Editor is a specialized view for node-based graph editing.
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
    static void DrawHydragonLine(
        ImDrawList* drawList,
        const ImVec2& start,
        const ImVec2& end,
        ImU32 color,
        float thickness);
    
    static void DrawSteppedLine(
        ImDrawList* drawList,
        const ImVec2& start,
        const ImVec2& end,
        ImU32 color,
        float thickness);
};

//------------------------------------------------------------------------------
template <typename T>
struct ImSteppedLinePointsT
{
    T Start;      // Starting point
    T Mid;        // Middle point where the step occurs
    T End;        // End point
    float Step;   // Step position (0.0f-1.0f)
    bool HorizontalFirst; // True: horizontal then vertical, False: vertical then horizontal
};
using ImSteppedLinePoints = ImSteppedLinePointsT<ImVec2>;

//------------------------------------------------------------------------------
// Core sampling functions
template <typename T> 
T ImSteppedLineSample(const T& start, const T& end, float t, float stepPosition = 0.5f, bool horizontalFirst = true);

template <typename T> 
T ImSteppedLineSample(const ImSteppedLinePointsT<T>& line, float t);

// Calculate exact length of stepped line
template <typename T> 
float ImSteppedLineLength(const T& start, const T& end, bool horizontalFirst = true);

template <typename T> 
float ImSteppedLineLength(const ImSteppedLinePointsT<T>& line);

// Calculate bounding rectangle
inline ImRect ImSteppedLineBoundingRect(const ImVec2& start, const ImVec2& end, bool horizontalFirst = true);
inline ImRect ImSteppedLineBoundingRect(const ImSteppedLinePoints& line);

// Project point on stepped line
struct ImSteppedLineProjectResult
{
    ImVec2 Point;    // Point on line
    float  Time;     // [0 - 1]
    float  Distance; // Distance to line
};

inline ImSteppedLineProjectResult ImProjectOnSteppedLine(
    const ImVec2& p, 
    const ImVec2& start, 
    const ImVec2& end, 
    bool horizontalFirst = true,
    float stepPosition = 0.5f
);

inline ImSteppedLineProjectResult ImProjectOnSteppedLine(
    const ImVec2& p, 
    const ImSteppedLinePoints& line
);

// Fixed step sampling for rendering
struct ImSteppedLineFixedStepSample
{
    ImVec2 Point;
    float  Length;
    bool   IsCorner;
};

using ImSteppedLineFixedStepCallback = void (*)(const ImSteppedLineFixedStepSample& p, void* user_pointer);

inline void ImSteppedLineFixedStep(
    ImSteppedLineFixedStepCallback callback,
    void* user_pointer,
    const ImVec2& start,
    const ImVec2& end,
    float step_position = 0.5f,
    bool horizontal_first = true
);

inline void ImSteppedLineFixedStep(
    ImSteppedLineFixedStepCallback callback,
    void* user_pointer,
    const ImSteppedLinePoints& line
);

// Template version for modern C++ callbacks
template <typename F>
inline void ImSteppedLineFixedStep(
    F& callback,
    const ImVec2& start,
    const ImVec2& end,
    float step_position = 0.5f,
    bool horizontal_first = true
);

template <typename F>
inline void ImSteppedLineFixedStep(F& callback, const ImSteppedLinePoints& line);

#endif // __IMGUI_STEPPED_LINE_MATH_H__
