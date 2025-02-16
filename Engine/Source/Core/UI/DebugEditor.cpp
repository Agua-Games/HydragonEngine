/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "DebugEditor.h"
#include "hdImgui.h"

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
                if (ImGui::MenuItem("New Debug Session")) {}
                if (ImGui::MenuItem("Import Debug Session", "Ctrl+O")) {}
                if (ImGui::MenuItem("Open Debug Session", "Ctrl+A")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Alt+F4")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Go to Line", "CTRL+Z")) {}
                if (ImGui::MenuItem("Find", "CTRL+Y", false, false)) {}  // Disabled item 
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Add Layer", "CTRL+A")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Settings")) {}    
                ImGui::EndMenu();
            }   
            ImGui::EndMenuBar();
        }
            
        ImGui::Button("Copy");
        ImGui::SameLine();
        ImGui::Button("Save");
        ImGui::SameLine();
        ImGui::Button("Clear");
        ImGui::SameLine();
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