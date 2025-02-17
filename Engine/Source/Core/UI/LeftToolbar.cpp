/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

#include "LeftToolbar.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {
void ShowLeftToolbar(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&window_class);

    // Style settings for a compact vertical toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
    
    ImVec2 buttonSize = windowData->iconDefaultSize;

    if (ImGui::Begin("Left Toolbar", p_open, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoScrollWithMouse))
    {
        // Selection Tools
        if (ImGui::Button(ICON_MS_SELECT_ALL "##Select", buttonSize)) { /* Handle Select */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select Tool");
        
        if (ImGui::Button(ICON_MS_HIGHLIGHT_ALT "##SelectRect", buttonSize)) { /* Handle Rectangle Select */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Rectangle Select");
        
        if (ImGui::Button(ICON_MS_RADIO_BUTTON_CHECKED "##SelectCircle", buttonSize)) { /* Handle Circle Select */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Circle Select");
        
        if (ImGui::Button(ICON_MS_POLYLINE "##SelectLasso", buttonSize)) { /* Handle Lasso Select */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Lasso Select");

        ImGui::Separator();

        // Transform Tools
        if (ImGui::Button(ICON_MS_OPEN_WITH "##Move", buttonSize)) { /* Handle Move */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Move Tool");
        
        if (ImGui::Button(ICON_MS_ROTATE_RIGHT "##Rotate", buttonSize)) { /* Handle Rotate */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Rotate Tool");
        
        if (ImGui::Button(ICON_MS_TRANSFORM "##Scale", buttonSize)) { /* Handle Scale */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scale Tool");

        ImGui::Separator();

        // Modeling Tools
        if (ImGui::Button(ICON_MS_ADD_BOX "##Create", buttonSize)) { /* Handle Create Primitive */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Create Primitive");
        
        if (ImGui::Button(ICON_MS_EDIT "##Edit", buttonSize)) { /* Handle Edit Geometry */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Edit Geometry");
        
        if (ImGui::Button(ICON_MS_TEXTURE "##UV", buttonSize)) { /* Handle UV Editor */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("UV Editor");

        ImGui::Separator();

        // Scene Tools
        if (ImGui::Button(ICON_MS_LIGHT "##Light", buttonSize)) { /* Handle Light Tool */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Light Tool");
        
        if (ImGui::Button(ICON_MS_CAMERA "##Camera", buttonSize)) { /* Handle Camera Tool */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Camera Tool");
        
        if (ImGui::Button(ICON_MS_BLUR_CIRCULAR "##Particle", buttonSize)) { /* Handle Particle Tool */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Particle System");

        ImGui::Separator();

        // Utility Tools
        if (ImGui::Button(ICON_MS_STRAIGHTEN "##Measure", buttonSize)) { /* Handle Measure Tool */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Measure Tool");
        
        if (ImGui::Button(ICON_MS_COMMENT "##Annotate", buttonSize)) { /* Handle Annotation Tool */ }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Annotations");
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
}
} // namespace hdImgui
