/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <vector>
#include <algorithm>

#include <imgui.h>
#include "IconsMaterialSymbols.h"

#include "FontEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowFontEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open || !*p_open)
        return;
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);

    bool window_visible = ImGui::Begin("Font", p_open, ImGuiWindowFlags_MenuBar);
    if (!window_visible)
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import Font", "Ctrl+I")) {}
            if (ImGui::MenuItem("Import Font Icons", "Ctrl+Shift+I")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save Font Settings", "Ctrl+S")) {}
            if (ImGui::MenuItem("Export Font Package", "Ctrl+E")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Preferences", "Ctrl+P")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 150.0f;
    const float maxLeftPanelWidth = 400.0f;
    
    // Left Panel - Font List and Properties
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);

    // Toolbar (fixed height section)
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_ADD_BOX "##Add", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Font");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_DELETE "##Remove", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Remove Font");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_REFRESH "##Refresh", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Refresh Font List");
    ImGui::PopStyleVar();
    ImGui::Separator();

    // Calculate remaining height for scrollable sections
    float availHeight = ImGui::GetContentRegionAvail().y;
    float fontListHeight = 200.0f; // Fixed height for font list
    float propertiesHeight = availHeight - fontListHeight - ImGui::GetStyle().ItemSpacing.y;


    // Font List (fixed height)
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Installed Fonts");
    ImGui::BeginChild("FontList", ImVec2(0, fontListHeight), true);
    {
        static int selected = 0;
        ImGui::Selectable("Roboto Regular", selected == 0);
        ImGui::Selectable("Roboto Bold", selected == 1);
        ImGui::Selectable("FontAwesome Icons", selected == 2);
    }
    ImGui::EndChild();

    // Properties Section (uses remaining height)
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Properties");
    ImGui::BeginChild("Properties", ImVec2(0, propertiesHeight), true);
    {
        static char fontNameBuffer[256] = "";  // Add this static buffer
        ImGui::Text("Font Name:");
        ImGui::InputText("##name", fontNameBuffer, IM_ARRAYSIZE(fontNameBuffer));
        
        ImGui::Text("Size:");
        static float fontSize = 16.0f;
        ImGui::DragFloat("##size", &fontSize, 0.5f, 4.0f, 72.0f);
        
        ImGui::Text("Style:");
        const char* styles[] = { "Regular", "Bold", "Italic", "Bold Italic" };
        static int styleIdx = 0;
        ImGui::Combo("##style", &styleIdx, styles, IM_ARRAYSIZE(styles));
        
        ImGui::Separator();
        
        ImGui::Text("Rendering:");
        static bool antiAliasing = true;
        ImGui::Checkbox("Anti-aliasing", &antiAliasing);
        
        static bool autoHinting = true;
        ImGui::Checkbox("Auto-hinting", &autoHinting);
        
        ImGui::Text("DPI Scale:");
        static float dpiScale = 1.0f;
        ImGui::DragFloat("##dpi", &dpiScale, 0.1f, 0.5f, 2.0f);
    }
    ImGui::EndChild();

    ImGui::EndChild(); // End LeftPanel

    ImGui::SameLine();

    // Resizer
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
    ImGui::Button("##Resizer", ImVec2(4.0f, -1));
    if (ImGui::IsItemActive())
    {
        leftPanelWidth += ImGui::GetIO().MouseDelta.x;
        leftPanelWidth = std::clamp(leftPanelWidth, minLeftPanelWidth, maxLeftPanelWidth);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    // Right Panel - Font Categories and Quick Access
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);

    // Tabs for different font categories
    if (ImGui::BeginTabBar("FontCategories"))
    {
        if (ImGui::BeginTabItem("System Fonts"))
        {
            ImGui::TextWrapped("System fonts will be listed here.\nConnect to NodeGraphEditor for font processing and preview.");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Icon Fonts"))
        {
            ImGui::TextWrapped("Icon fonts (FontAwesome, Material Icons, etc.) will be listed here.");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Custom Fonts"))
        {
            ImGui::TextWrapped("User-imported custom fonts will appear here.");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }  
    ImGui::EndChild();  // End RightPanel

    ImGui::End();  // End Font Editor window
}
} // namespace hdImgui
