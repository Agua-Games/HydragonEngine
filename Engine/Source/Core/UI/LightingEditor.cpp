/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include "LightingEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

void ShowLightingEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    if (!ImGui::Begin("Lighting", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Light Setup", "Ctrl+N")) {}
            if (ImGui::MenuItem("Import Light Setup...", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Analysis"))
        {
            if (ImGui::MenuItem("Light Overlap Check")) {}
            if (ImGui::MenuItem("Shadow Quality Analysis")) {}
            if (ImGui::MenuItem("Performance Profiler")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Generate Light Report")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Toolbar
    ImGui::BeginChild("LightingToolbar", ImVec2(-1, 35), false);
    {
        if (ImGui::Button(ICON_MS_ADD "##AddLight", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Light");
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_DELETE "##DeleteLight", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Light");
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_CONTENT_COPY "##DuplicateLight", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Duplicate Light");
        ImGui::SameLine();

        ImGui::Dummy(ImVec2(10, 0));
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_LIGHTBULB "##ToggleAll", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle All Lights");
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_AUTO_FIX_HIGH "##AutoSetup", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Auto Setup Lighting");
    }
    ImGui::EndChild();

    // Main Content
    ImGui::BeginChild("LightingContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); 

    // Store the current sidebar width in a static variable
    static float leftPanelWidth = 200.0f;
    const float minLeftPanelWidth = 150.0f;
    const float maxLeftPanelWidth = 400.0f;

    // Light List Panel
    ImGui::BeginChild("LightList", ImVec2(leftPanelWidth, 0), true);
    ImGui::Text("Lights");
    ImGui::Separator();
    
    if (ImGui::Button("Add Light", ImVec2(-1, 0))) {}
    
    ImGui::Separator();
    if (ImGui::Selectable("Main Light", true)) {}
    if (ImGui::Selectable("Fill Light", false)) {}
    if (ImGui::Selectable("Rim Light", false)) {}
    if (ImGui::Selectable("Ambient Light", false)) {}
    ImGui::EndChild();

    ImGui::SameLine();

    // Splitter
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
    ImGui::Button("##Splitter", ImVec2(8.0f, -1));
    ImGui::PopStyleColor(3);

    if (ImGui::IsItemActive())
    {
        float deltaX = ImGui::GetIO().MouseDelta.x;
        leftPanelWidth += deltaX;
        if (leftPanelWidth < minLeftPanelWidth) leftPanelWidth = minLeftPanelWidth;
        if (leftPanelWidth > maxLeftPanelWidth) leftPanelWidth = maxLeftPanelWidth;
    }
    else if (ImGui::IsItemHovered())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    }

    ImGui::SameLine();

    // Properties Panel
    ImGui::BeginChild("Properties", ImVec2(0, 0), true);
    ImGui::Text("Light Properties");
    ImGui::Separator();
    
    static float intensity = 1.0f;
    static float range = 10.0f;
    static float innerConeAngle = 30.0f;
    static float outerConeAngle = 45.0f;
    static float temperature = 6500.0f;
    static float color[3] = {1.0f, 1.0f, 1.0f};
    
    ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f);
    ImGui::SliderFloat("Range", &range, 0.0f, 100.0f);
    ImGui::SliderFloat("Inner Cone Angle", &innerConeAngle, 0.0f, 90.0f);
    ImGui::SliderFloat("Outer Cone Angle", &outerConeAngle, 0.0f, 90.0f);
    ImGui::SliderFloat("Temperature", &temperature, 1000.0f, 12000.0f);
    ImGui::ColorEdit3("Color", color);
    
    ImGui::Separator();
    ImGui::Text("Shadows");
    static bool castShadows = true;
    static float shadowBias = 0.05f;
    static int shadowResolution = 1024;
    
    ImGui::Checkbox("Cast Shadows", &castShadows);
    ImGui::SliderFloat("Shadow Bias", &shadowBias, 0.0f, 1.0f);
    ImGui::SliderInt("Shadow Resolution", &shadowResolution, 256, 4096);
    
    ImGui::EndChild();
    
    ImGui::EndChild();
    
    // Status Bar
    ImGui::BeginChild("StatusBar", ImVec2(0, ImGui::GetFrameHeightWithSpacing()), true);
    ImGui::Text("Selected: Main Light  |  Mode: Editing  |  FPS: 60");
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
