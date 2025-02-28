#pragma once

#include <imgui.h>

// Result structure for projection calculations
struct ImSteppedLineProjectResult {
    ImVec2 point;     // Projected point on the line
    float distance;   // Distance from input point to projected point
    float t;          // Normalized position along the line (0.0 to 1.0)
};

// Main class declaration
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

    static ImSteppedLineProjectResult ProjectOnLine(
        const ImVec2& point,
        const ImVec2& start,
        const ImVec2& end);

private:
    static void AddArcPoints(
        ImVec2* points,
        int& pointCount,
        const ImVec2& cornerCenter,
        float radius,
        float startAngle,
        float endAngle,
        int numSegments = 8);
};
