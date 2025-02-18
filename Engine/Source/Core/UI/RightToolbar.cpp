/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

#include "RightToolbar.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {
void ShowRightToolbar(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&window_class);

    // Style settings for a compact vertical toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
    
    ImVec2 buttonSize = windowData->iconDefaultSize;

    if (ImGui::Begin("Right Toolbar", p_open, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoScrollWithMouse))
    {
        // Scene Management Tools
        if (ImGui::Button(ICON_MS_LAYERS "##Layers", buttonSize)) { /* Handle Layers Panel */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Layers");

        if (ImGui::Button(ICON_MS_SETTINGS "##Properties", buttonSize)) { /* Handle Properties Panel */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Properties");

        ImGui::Separator();

        // Asset Management
        if (ImGui::Button(ICON_MS_FOLDER "##Assets", buttonSize)) { /* Handle Asset Browser */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Asset Browser");

        if (ImGui::Button(ICON_MS_PALETTE "##Materials", buttonSize)) { /* Handle Material Editor */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Materials");

        ImGui::Separator();

        // Scene Preview
        if (ImGui::Button(ICON_MS_PREVIEW "##Preview", buttonSize)) { /* Handle Scene Preview */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scene Preview");
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
}
} // namespace hdImgui
