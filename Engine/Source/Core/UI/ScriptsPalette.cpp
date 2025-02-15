/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ScriptsPalette.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowScriptsPalette(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Scripts Palette", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginTabBar("Scripts Palette Tabs"))
        {
            if (ImGui::BeginTabItem("Hydragon"))
            {
                if (ImGui::CollapsingHeader("Physics")) {
                    ImGui::Button("Optimize Simulation For Solver");
                    ImGui::Button("Setup Basic Destructible");
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("User"))
            {
                ImGui::Button("Add Script");    // Migrate to consolidated Menu bar later
                ImGui::Button("Customize UI for my project"); 
                ImGui::Button("Add My Project's Macros"); 
                ImGui::Button("Add, Open My Project's UI"); 
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}
} // namespace hdImgui