/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScriptsPalette.cpp
 * @brief The Scripts Palette is a specialized view for displaying and executing scripts.
 */
#pragma once
#include <imgui.h>
#include <string>

#include "ScriptsPalette.h"
#include "MainEditorUIManager.h"

namespace hd {

void showScriptsPalette(bool* p_open, EditorWindowData* windowData) 
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
} // namespace hd