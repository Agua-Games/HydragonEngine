#include "Imgui_SteppedLineMath.h"
#include <algorithm>

//------------------------------------------------------------------------------
template <typename T>
T ImSteppedLineSample(const T& start, const T& end, float t, float stepPosition, bool horizontalFirst)
{
    IM_ASSERT(t >= 0.0f && t <= 1.0f);
    IM_ASSERT(stepPosition >= 0.0f && stepPosition <= 1.0f);

    if (t <= 0.0f) return start;
    if (t >= 1.0f) return end;

    T result;
    if (horizontalFirst)
    {
        // First segment: horizontal movement
        if (t < stepPosition)
        {
            float segmentT = t / stepPosition;
            result.x = start.x + (end.x - start.x) * segmentT;
            result.y = start.y;
        }
        // Second segment: vertical movement
        else
        {
            float segmentT = (t - stepPosition) / (1.0f - stepPosition);
            result.x = end.x;
            result.y = start.y + (end.y - start.y) * segmentT;
        }
    }
    else
    {
        // First segment: vertical movement
        if (t < stepPosition)
        {
            float segmentT = t / stepPosition;
            result.x = start.x;
            result.y = start.y + (end.y - start.y) * segmentT;
        }
        // Second segment: horizontal movement
        else
        {
            float segmentT = t / stepPosition;
            result.x = start.x + (end.x - start.x) * segmentT;
            result.y = end.y;
        }
    }
    return result;
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
    // For stepped lines, length is simply sum of absolute differences
    return std::abs(end.x - start.x) + std::abs(end.y - start.y);
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
    const float step = 1.0f / 20.0f; // Adjust number of segments as needed
    
    ImSteppedLineFixedStepSample sample;
    sample.IsCorner = false;

    // First point
    sample.Point = start;
    sample.Length = 0.0f;
    callback(sample, user_pointer);

    // Step point (corner)
    float t = step_position;
    sample.Point = ImSteppedLineSample(start, end, t, step_position, horizontal_first);
    sample.IsCorner = true;
    sample.Length = t * ImSteppedLineLength(start, end, horizontal_first);
    callback(sample, user_pointer);

    // End point
    sample.Point = end;
    sample.IsCorner = false;
    sample.Length = ImSteppedLineLength(start, end, horizontal_first);
    callback(sample, user_pointer);
}

//------------------------------------------------------------------------------
ImSteppedLineProjectResult ImProjectOnSteppedLine(
    const ImVec2& p, 
    const ImVec2& start, 
    const ImVec2& end, 
    bool horizontalFirst,
    float stepPosition,
    float cornerRadius = 0.0f)
{
    ImSteppedLineProjectResult result;
    result.Distance = FLT_MAX;

    // Helper to update result if new distance is shorter
    auto updateResult = [&result](const ImVec2& point, float time, float distance) {
        if (distance < result.Distance) {
            result.Point = point;
            result.Time = time;
            result.Distance = distance;
        }
    };

    // Calculate corner point
    ImVec2 corner;
    if (horizontalFirst) {
        corner = ImVec2(end.x, start.y);
    } else {
        corner = ImVec2(start.x, end.y);
    }

    // If we have a corner radius, adjust the segments and add corner projection
    if (cornerRadius > 0.0f) {
        // Calculate corner center (shifted inward by radius)
        ImVec2 cornerCenter = corner;
        if (horizontalFirst) {
            if (end.x < start.x) cornerCenter.x += cornerRadius;
            else cornerCenter.x -= cornerRadius;
            if (end.y < start.y) cornerCenter.y -= cornerRadius;
            else cornerCenter.y += cornerRadius;
        } else {
            if (end.x < start.x) cornerCenter.x -= cornerRadius;
            else cornerCenter.x += cornerRadius;
            if (end.y < start.y) cornerCenter.y += cornerRadius;
            else cornerCenter.y -= cornerRadius;
        }

        // Project onto corner arc
        ImVec2 toPoint = p - cornerCenter;
        float distToCorner = ImLength(toPoint);
        if (distToCorner > 0.0f) {
            ImVec2 projected = cornerCenter + (toPoint * (cornerRadius / distToCorner));
            
            // Calculate angle to determine if projection is within arc bounds
            float angle = atan2f(toPoint.y, toPoint.x); // Changed from atan2 to atan2f
            bool isInArc = false;
            float startAngle, endAngle; // Declare these variables before use
            
            if (horizontalFirst) {
                startAngle = (end.y > start.y) ? 0.0f : -IM_PI/2.0f;
                endAngle = (end.y > start.y) ? IM_PI/2.0f : 0.0f;
                isInArc = (angle >= startAngle && angle <= endAngle);
            } else {
                startAngle = (end.x > start.x) ? -IM_PI/2.0f : -IM_PI;
                endAngle = (end.x > start.x) ? 0.0f : -IM_PI/2.0f;
                isInArc = (angle >= startAngle && angle <= endAngle);
            }

            if (isInArc) {
                float arcLength = cornerRadius * (IM_PI/2.0f); // Added parentheses for clarity
                float totalLength = ImSteppedLineLength(start, end, horizontalFirst);
                float t = stepPosition;  // Base time at corner
                
                // Adjust time based on angle along arc
                float angleNormalized = (angle - startAngle) / (IM_PI/2.0f);
                t += angleNormalized * (arcLength / totalLength) * (1.0f - stepPosition);
                
                float dist = ImLengthSqr(p - projected);
                updateResult(projected, t, dist);
            }
        }

        // Adjust segment endpoints for radius
        if (horizontalFirst) {
            // Shorten horizontal segment
            if (end.x < start.x) corner.x += cornerRadius;
            else corner.x -= cornerRadius;
        } else {
            // Shorten vertical segment
            if (end.y < start.y) corner.y += cornerRadius;
            else corner.y -= cornerRadius;
        }
    }

    // Project onto first segment
    if (horizontalFirst) {
        // Horizontal segment
        float x = ImClamp(p.x, start.x, corner.x);
        ImVec2 projected(x, start.y);
        float t = ImFabs(corner.x - start.x) > 0.0f ? 
            (x - start.x) / (end.x - start.x) * stepPosition : 0.0f;
        float dist = ImLengthSqr(p - projected);
        updateResult(projected, t, dist);
    } else {
        // Vertical segment
        float y = ImClamp(p.y, start.y, corner.y);
        ImVec2 projected(start.x, y);
        float t = ImFabs(corner.y - start.y) > 0.0f ? 
            (y - start.y) / (end.y - start.y) * stepPosition : 0.0f;
        float dist = ImLengthSqr(p - projected);
        updateResult(projected, t, dist);
    }

    // Project onto second segment
    ImVec2 segEnd = end;
    if (horizontalFirst) {
        // Vertical segment
        if (end.y < start.y) segEnd.y = corner.y - cornerRadius;
        else segEnd.y = corner.y + cornerRadius;
        
        float y = ImClamp(p.y, corner.y, segEnd.y);
        ImVec2 projected(end.x, y);
        float t = stepPosition + (ImFabs(segEnd.y - corner.y) > 0.0f ? 
            (y - corner.y) / (end.y - start.y) * (1.0f - stepPosition) : 0.0f);
        float dist = ImLengthSqr(p - projected);
        updateResult(projected, t, dist);
    } else {
        // Horizontal segment
        if (end.x < start.x) segEnd.x = corner.x - cornerRadius;
        else segEnd.x = corner.x + cornerRadius;
        
        float x = ImClamp(p.x, corner.x, segEnd.x);
        ImVec2 projected(x, end.y);
        float t = stepPosition + (ImFabs(segEnd.x - corner.x) > 0.0f ? 
            (x - corner.x) / (end.x - start.x) * (1.0f - stepPosition) : 0.0f);
        float dist = ImLengthSqr(p - projected);
        updateResult(projected, t, dist);
    }

    // Take square root of final distance
    result.Distance = ImSqrt(result.Distance);
    return result;
}

//------------------------------------------------------------------------------
ImSteppedLineProjectResult ImProjectOnSteppedLine(
    const ImVec2& p, 
    const ImSteppedLinePoints& line,
    float cornerRadius = 0.0f)
{
    return ImProjectOnSteppedLine(p, line.Start, line.End, line.HorizontalFirst, line.Step, cornerRadius);
}

void ImSteppedLineRenderer::DrawLine(
    ImDrawList* drawList,
    const ImVec2& start,
    const ImVec2& end,
    ImU32 color,
    float thickness,
    const ImSteppedLineStyle& style)
{
    // Just draw a straight line from start to end
    drawList->AddLine(start, end, color, thickness);
}
