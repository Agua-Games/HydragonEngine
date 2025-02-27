#include "Imgui_SteppedLineRenderer.h"
#include "Imgui_SteppedLineMath.h"  // Internal dependency
#include <cmath>  // For mathematical operations

void ImSteppedLineRenderer::DrawLine(
    ImDrawList* drawList,
    const ImVec2& start,
    const ImVec2& end,
    ImU32 color,
    float thickness,
    const ImSteppedLineStyle& style)
{
    const int MAX_POINTS = 22; // Maximum points needed (20 segments + start + end)
    ImVec2 points[MAX_POINTS];
    int pointCount = 0;
    
    // Start point
    points[pointCount++] = start;

    if (style.cornerRadius <= 0.0f) {
        // Simple stepped line without rounded corners
        ImVec2 corner = style.horizontalFirst ? 
            ImVec2(end.x, start.y) : 
            ImVec2(start.x, end.y);
        points[pointCount++] = corner;
        points[pointCount++] = end;
    } else {
        // Calculate corner position
        ImVec2 corner = style.horizontalFirst ? 
            ImVec2(end.x, start.y) : 
            ImVec2(start.x, end.y);
            
        // Calculate corner center (shifted inward by radius)
        ImVec2 cornerCenter = corner;
        if (style.horizontalFirst) {
            cornerCenter.x += (end.x < start.x) ? style.cornerRadius : -style.cornerRadius;
            cornerCenter.y += (end.y < start.y) ? -style.cornerRadius : style.cornerRadius;
        } else {
            cornerCenter.x += (end.x < start.x) ? -style.cornerRadius : style.cornerRadius;
            cornerCenter.y += (end.y < start.y) ? style.cornerRadius : -style.cornerRadius;
        }

        // Add first segment endpoint
        ImVec2 arcStart = style.horizontalFirst ?
            ImVec2(cornerCenter.x + ((end.x < start.x) ? -style.cornerRadius : style.cornerRadius), start.y) :
            ImVec2(start.x, cornerCenter.y + ((end.y < start.y) ? -style.cornerRadius : style.cornerRadius));
        points[pointCount++] = arcStart;

        // Add arc points
        float startAngle, endAngle;
        if (style.horizontalFirst) {
            startAngle = (end.y > start.y) ? 0.0f : -IM_PI/2.0f;
            endAngle = (end.y > start.y) ? IM_PI/2.0f : 0.0f;
        } else {
            startAngle = (end.x > start.x) ? -IM_PI/2.0f : -IM_PI;
            endAngle = (end.x > start.x) ? 0.0f : -IM_PI/2.0f;
        }

        AddArcPoints(points, pointCount, cornerCenter, style.cornerRadius, startAngle, endAngle);
    }
    
    // End point
    points[pointCount++] = end;

    // Draw the segments
    drawList->AddPolyline(points, pointCount, color, false, thickness);
}

void ImSteppedLineRenderer::AddArcPoints(
    ImVec2* points,
    int& pointCount,
    const ImVec2& cornerCenter,
    float radius,
    float startAngle,
    float endAngle,
    int numSegments)
{
    for (int i = 0; i <= numSegments; i++) {
        float t = i / static_cast<float>(numSegments);
        float angle = startAngle + (endAngle - startAngle) * t;
        points[pointCount++] = ImVec2(
            cornerCenter.x + cosf(angle) * radius,
            cornerCenter.y + sinf(angle) * radius
        );
    }
}

ImSteppedLineProjectResult ImSteppedLineRenderer::ProjectOnLine(
    const ImVec2& point,
    const ImVec2& start,
    const ImVec2& end,
    const ImSteppedLineStyle& style)
{
    return ImProjectOnSteppedLine(point, start, end, 
        style.horizontalFirst, style.stepPosition, style.cornerRadius);
}
