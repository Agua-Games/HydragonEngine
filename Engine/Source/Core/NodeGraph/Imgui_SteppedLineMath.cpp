#include "Imgui_SteppedLineMath.h"
#include <algorithm>

//------------------------------------------------------------------------------
template <typename T>
T ImSteppedLineSample(const T& start, const T& end, float t, float stepPosition, bool horizontalFirst)
{
    // Simple linear interpolation
    return start + (end - start) * t;
}

//------------------------------------------------------------------------------
template <typename T>
T ImSteppedLineSample(const ImSteppedLinePointsT<T>& line, float t)
{
    return ImSteppedLineSample(line.Start, line.End, t, line.Step, line.HorizontalFirst);
}

//------------------------------------------------------------------------------
template <typename T>
float ImSteppedLineLength(const T& start, const T& end, bool horizontalFirst)
{
    // Simple distance calculation
    return ImLength(end - start);
}

//------------------------------------------------------------------------------
template <typename T>
float ImSteppedLineLength(const ImSteppedLinePointsT<T>& line)
{
    return ImSteppedLineLength(line.Start, line.End, line.HorizontalFirst);
}

//------------------------------------------------------------------------------
ImRect ImSteppedLineBoundingRect(const ImVec2& start, const ImVec2& end, bool horizontalFirst)
{
    ImRect rect;
    rect.Add(start);
    rect.Add(end);
    return rect;
}

//------------------------------------------------------------------------------
ImRect ImSteppedLineBoundingRect(const ImSteppedLinePoints& line)
{
    return ImSteppedLineBoundingRect(line.Start, line.End, line.HorizontalFirst);
}

//------------------------------------------------------------------------------
void ImSteppedLineFixedStep(
    ImSteppedLineFixedStepCallback callback,
    void* user_pointer,
    const ImVec2& start,
    const ImVec2& end,
    float step_position,
    bool horizontal_first)
{
    ImSteppedLineFixedStepSample sample;
    
    // Just sample start and end points
    sample.Point = start;
    sample.Length = 0.0f;
    sample.IsCorner = false;
    callback(sample, user_pointer);
    
    sample.Point = end;
    sample.Length = ImLength(end - start);
    sample.IsCorner = false;
    callback(sample, user_pointer);
}

//------------------------------------------------------------------------------
ImSteppedLineProjectResult ImProjectOnSteppedLine(
    const ImVec2& p, 
    const ImVec2& start, 
    const ImVec2& end, 
    bool horizontalFirst,
    float stepPosition,
    float cornerRadius)
{
    ImSteppedLineProjectResult result;
    
    // Simple projection onto line
    ImVec2 line = end - start;
    float len2 = ImLengthSqr(line);
    
    if (len2 < FLT_EPSILON)
    {
        result.Point = start;
        result.Time = 0;
        result.Distance = ImLength(p - start);
        return result;
    }
    
    float t = ImClamp(ImDot(p - start, line) / len2, 0.0f, 1.0f);
    result.Point = start + line * t;
    result.Time = t;
    result.Distance = ImLength(p - result.Point);
    
    return result;
}

//------------------------------------------------------------------------------
ImSteppedLineProjectResult ImProjectOnSteppedLine(
    const ImVec2& p, 
    const ImSteppedLinePoints& line,
    float cornerRadius)
{
    return ImProjectOnSteppedLine(p, line.Start, line.End, line.HorizontalFirst, line.Step, cornerRadius);
}

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
