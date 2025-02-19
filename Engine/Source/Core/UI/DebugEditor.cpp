/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <string>

#include "DebugEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

float filter_InputTextWidth = 500.0f;

void ShowDebugEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Debug", p_open, ImGuiWindowFlags_MenuBar))
    {
        bool AutoScroll = true;
        static char InputBuf[256];

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem(ICON_MS_ADD_BOX " New Debug Session")) {}
                if (ImGui::MenuItem(ICON_MS_FOLDER_OPEN " Import Debug Session", "Ctrl+O")) {}
                if (ImGui::MenuItem(ICON_MS_DOWNLOAD " Open Debug Session", "Ctrl+A")) {}
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_MS_LOGOUT " Exit", "Alt+F4")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem(ICON_MS_CONTENT_CUT " Cut", "CTRL+X")) {}
                if (ImGui::MenuItem(ICON_MS_CONTENT_COPY " Copy", "CTRL+C")) {}
                if (ImGui::MenuItem(ICON_MS_CONTENT_PASTE " Paste", "CTRL+V")) {}
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_MS_KEYBOARD_RETURN " Go to Line", "CTRL+G")) {}
                if (ImGui::MenuItem(ICON_MS_SEARCH " Find", "CTRL+F", false, false)) {}  // Disabled item 
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem(ICON_MS_ADD_CIRCLE " Add Layer", "CTRL+L")) {}
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_MS_SETTINGS " Settings")) {}    
                ImGui::EndMenu();
            }   
            ImGui::EndMenuBar();
        }
            
        // Toolbar with icons
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        
        ImVec2 buttonSize(32, 32);
        
        if (ImGui::Button(ICON_MS_CONTENT_COPY "##Copy", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Copy");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_SAVE "##Save", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save Debug Log");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_CLEAR "##Clear", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Clear Debug Log");
        ImGui::SameLine();
        
        ImGui::Dummy(ImVec2(10, 0));
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_PLAY_ARROW "##Play", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Continue");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_PAUSE "##Pause", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Pause");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_SKIP_NEXT "##StepOver", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Step Over");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_SUBDIRECTORY_ARROW_RIGHT "##StepInto", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Step Into");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_SUBDIRECTORY_ARROW_LEFT "##StepOut", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Step Out");
        ImGui::SameLine();
        
        ImGui::Dummy(ImVec2(10, 0));
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_BUG_REPORT "##BreakPoint", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Breakpoint");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_PSYCHOLOGY "##Evaluate", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Evaluate Expression");
        
        ImGui::PopStyleVar(2);
        ImGui::Separator();

        ImGui::SetNextItemWidth(filter_InputTextWidth);
        ImGui::InputText("Filter", InputBuf, IM_ARRAYSIZE(InputBuf));
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::SameLine();
        ImGui::Button("Debug Settings");
        ImGui::Separator();
    }      
    ImGui::End();
}
} // namespace hdImgui
