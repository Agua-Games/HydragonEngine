/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * he Imgui Stepped Line Math is a custom implementation for stepped line calculations.
 */

#include "Imgui_SteppedLineMath.h"
#include <algorithm>

void ImSteppedLineRenderer::DrawLinkShapeHandle(ImVec2 ratioPoint, ImDrawList* drawList, float size, ImU32 color)
{
    float squareSize = 5.0f;
    ImVec2 squareMin(ratioPoint.x - squareSize, ratioPoint.y - squareSize);
    ImVec2 squareMax(ratioPoint.x + squareSize, ratioPoint.y + squareSize);
    drawList->AddRect(squareMin, squareMax, color);
}

void ImSteppedLineRenderer::DrawHydragonLine(
    ImDrawList* drawList,
    const ImVec2& start,
    const ImVec2& end,
    ImU32 color,
    float thickness, 
    bool enableHandle)
{
    // Calculate the ratio point using golden ratio
    float ratio = 0.382034f;
    ImVec2 ratioPoint = start + (end - start) * ratio;
    
    // First segment (stepped): start to ratioPoint
    // Step 1: Draw horizontal line (x component only)
    ImVec2 horizontalEnd(ratioPoint.x, start.y);
    drawList->AddLine(start, horizontalEnd, color, thickness);
    
    // Step 2: Draw vertical line (y component only)
    drawList->AddLine(horizontalEnd, ratioPoint, color, thickness);
    
    // Second segment: straight line from ratioPoint to end
    drawList->AddLine(ratioPoint, end, color, thickness);
    
    // Draw the ratio point as a square handle, so user can move the ratioPoint?
    if (enableHandle)
    {
        DrawLinkShapeHandle(ratioPoint, drawList, 5.0f, color);
    }
    
}

void ImSteppedLineRenderer::DrawSteppedLine(
    ImDrawList* drawList,
    const ImVec2& start,
    const ImVec2& end,
    ImU32 color,
    float thickness,
    bool enableHandle)
{
    // Calculate the ratio point using inverse golden ratio
    float ratio = 0.382034f;
    ImVec2 ratioPoint = start + (end - start) * ratio;
    
    // First segment (stepped): start to ratioPoint
    // Step 1: Draw horizontal line (x component only)
    ImVec2 horizontalEnd1(ratioPoint.x, start.y);
    drawList->AddLine(start, horizontalEnd1, color, thickness);
    
    // Step 2: Draw vertical line (y component only)
    drawList->AddLine(horizontalEnd1, ratioPoint, color, thickness);
    
    // Second segment (stepped): ratioPoint to end
    // Step 1: Draw vertical line (y component only)
    ImVec2 horizontalEnd2(ratioPoint.x, end.y);
    drawList->AddLine(ratioPoint, horizontalEnd2, color, thickness);
    
    // Step 2: Draw horizontal line (x component only)
    drawList->AddLine(horizontalEnd2, end, color, thickness);
    
    // Draw the ratio point as a square handle, so user can move the ratioPoint?
    if (enableHandle)
    {
        DrawLinkShapeHandle(ratioPoint, drawList, 5.0f, color);
    }
}
