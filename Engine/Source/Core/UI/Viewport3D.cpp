/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

#include "Viewport3D.h"
#include "hdImgui.h"

namespace hdImgui {

// Viewport overlay states
static bool showStats = true;
static bool showGrid = true;

void ShowViewport3D(bool* p_open, HdEditorWindowData* windowData) 
{
    // Configure window to maximize space for 3D content
    ImGui::SetNextWindowBgAlpha(1.0f);
    
    ImGuiWindowFlags viewport_flags = ImGuiWindowFlags_MenuBar | 
                                    ImGuiWindowFlags_NoScrollbar | 
                                    ImGuiWindowFlags_NoScrollWithMouse |
                                    ImGuiWindowFlags_NoCollapse;
    
    if (ImGui::Begin("Viewport 3D", p_open, viewport_flags))
    {
        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Frame Selected", "F")) {}
                if (ImGui::MenuItem("Frame All", "A")) {}
                ImGui::Separator();
                ImGui::MenuItem("Statistics", "Ctrl+`", &showStats);
                ImGui::MenuItem("Grid", "G", &showGrid);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Camera"))
            {
                if (ImGui::MenuItem("Perspective", "5")) {}
                if (ImGui::MenuItem("Front", "1")) {}
                if (ImGui::MenuItem("Side", "3")) {}
                if (ImGui::MenuItem("Top", "7")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Save Camera...")) {}
                if (ImGui::MenuItem("Load Camera...")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Get viewport dimensions
        ImVec2 viewportPos = ImGui::GetCursorScreenPos();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        
        // Create a dummy button that covers the entire viewport area
        ImGui::InvisibleButton("##viewport", viewportSize);
        
        // Get draw list for overlays
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Draw viewport background
  /*       drawList->AddRectFilled(viewportPos, 
            ImVec2(viewportPos.x + viewportSize.x, viewportPos.y + viewportSize.y), 
            IM_COL32(51, 51, 51, 255)); */

        // Top-left: View mode and stats
        if (showStats)
        {
            ImVec2 statsPos = viewportPos;
            statsPos.x += 10;
            statsPos.y += 10;
            
            drawList->AddText(statsPos, 
                IM_COL32(255, 255, 255, 200), "Perspective");
            
            char stats[128];
            snprintf(stats, sizeof(stats), "FPS: %.1f | Tris: 1.2M | Lights: 4", 
                ImGui::GetIO().Framerate);
            drawList->AddText(ImVec2(statsPos.x, statsPos.y + 20), 
                IM_COL32(255, 255, 255, 200), stats);
        }

        // Bottom-left: Scene Axis indicator
        {
            ImVec2 axisOrigin = viewportPos;
            axisOrigin.x += 10;  // Reduced offset from left edge
            axisOrigin.y += viewportSize.y - 10;  // Reduced offset from bottom edge
            
            float axisLength = 20.0f;  // Reduced length
            float axisThickness = 1.0f;  // Slightly reduced thickness
            
            // X axis (desaturated red)
            drawList->AddLine(
                axisOrigin,
                ImVec2(axisOrigin.x + axisLength, axisOrigin.y),
                IM_COL32(180, 70, 70, 255),
                axisThickness
            );
            drawList->AddText(
                ImVec2(axisOrigin.x + axisLength + 3, axisOrigin.y - 7),
                IM_COL32(230, 100, 100, 255),
                "X"
            );
            
            // Y axis (desaturated green)
            drawList->AddLine(
                axisOrigin,
                ImVec2(axisOrigin.x, axisOrigin.y - axisLength),
                IM_COL32(70, 180, 70, 255),
                axisThickness
            );
            drawList->AddText(
                ImVec2(axisOrigin.x - 2, axisOrigin.y - axisLength - 15),
                IM_COL32(70, 180, 70, 255),
                "Y"
            );
            
            // Z axis (desaturated blue)
            drawList->AddLine(
                axisOrigin,
                ImVec2(axisOrigin.x + axisLength * 0.707f, axisOrigin.y - axisLength * 0.707f),
                IM_COL32(100, 100, 230, 255),
                axisThickness
            );
            drawList->AddText(
                ImVec2(axisOrigin.x + axisLength * 0.707f + 3, axisOrigin.y - axisLength * 0.707f - 9),
                IM_COL32(100, 100, 230, 255),
                "Z"
            );
        }
    }
    ImGui::End();
}

} // namespace hdImgui
