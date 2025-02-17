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
        float comboOverlaySpacing = 8;
        float comboOverlayCategorySpacing = 70;
        
        // Top viewport controls
        {
            float controlsY = viewportPos.y;
            float startX = viewportPos.x + (viewportSize.x * 0.1f); // Start at 20% from left for more space
            
            // Style settings for combos
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, windowData->globalWindowBgAlpha));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, windowData->globalWindowBgAlpha));
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, windowData->globalWindowBgAlpha));
            
            static int coordMode = 0;
            static int shadingMode = 0;
            static int lightingMode = 0;
            static int translateSnapMode = 0;
            static int translateSnapValue = 2; // Default to 1.0
            static int rotateSnapValue = 0;    // Default to none
            static int scaleSnapValue = 0;     // Default to none

            // Position transform controls
            ImGui::SetCursorScreenPos(ImVec2(startX, controlsY));

            // First row - all in one line with proper spacing
            ImGui::Text("Coord"); ImGui::SameLine(0, 4);
            ImGui::SetNextItemWidth(60);
            ImGui::Combo("##Coordinates", &coordMode, "Global\0Local\0");
            
            // Rendering controls
            ImGui::SameLine(0, comboOverlayCategorySpacing);
            ImGui::Text("Shading"); ImGui::SameLine(0, 4);
            ImGui::SetNextItemWidth(120);
            ImGui::Combo("##Shading", &shadingMode, "Wireframe\0Shaded\0Shaded & Textured\0Matcap\0"); ImGui::SameLine(0, comboOverlaySpacing);
            
            ImGui::Text("Lighting"); ImGui::SameLine(0, 4);
            ImGui::SetNextItemWidth(100);
            ImGui::Combo("##Lighting", &lightingMode, "Lit\0Unlit\0Lit + Detail\0");

            // Snapping controls
            ImGui::SameLine(0, comboOverlayCategorySpacing);
            ImGui::Text("Snap to"); ImGui::SameLine(0, 4);
            ImGui::SetNextItemWidth(80);
            ImGui::Combo("##TranslateSnapMode", &translateSnapMode, "None\0Grid\0Vertex\0Corner\0"); ImGui::SameLine(0, comboOverlaySpacing);
            
            ImGui::Text("Transl"); ImGui::SameLine(0, 4);
            ImGui::SetNextItemWidth(80);
            ImGui::Combo("##TranslateSnap", &translateSnapValue, "0.01\0" "0.1\0" "1.0\0" "10.0\0" "100.0\0"); ImGui::SameLine(0, comboOverlaySpacing);
            
            ImGui::Text("Rot"); ImGui::SameLine(0, 4);
            ImGui::SetNextItemWidth(80);
            ImGui::Combo("##RotateSnap", &rotateSnapValue, "None\0" "1°\0" "5°\0" "10°\0" "15°\0" "20°\0" "25°\0" "30°\0" "90°\0"); ImGui::SameLine(0, comboOverlaySpacing);
            
            ImGui::Text("Scale"); ImGui::SameLine(0, 4);
            ImGui::SetNextItemWidth(80);
            ImGui::Combo("##ScaleSnap", &scaleSnapValue, "None\0" "0.5\0" "1.0\0" "2.0\0" "3.0\0" "4.0\0" "5.0\0" "10.0\0");

            // Pop styles
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(3);
        }

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
