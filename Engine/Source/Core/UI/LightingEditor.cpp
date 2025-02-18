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
    if (!p_open) return;
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Lighting Editor", p_open))
    {
        // Left panel (fixed width)
        ImGui::BeginChild("LeftPanel", ImVec2(320.0f, 0), true);
        {
            // Simple table with headers
            if (ImGui::BeginTable("LightsTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Vis", ImGuiTableColumnFlags_WidthFixed, 26.0f);
                ImGui::TableSetupColumn("Shd", ImGuiTableColumnFlags_WidthFixed, 26.0f);
                ImGui::TableSetupColumn("Int", ImGuiTableColumnFlags_WidthFixed, 40.0f);
                ImGui::TableHeadersRow();
                
                // Simple row as placeholder
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Main Light");
                ImGui::TableNextColumn();
                ImGui::Text("V");
                ImGui::TableNextColumn();
                ImGui::Text("S");
                ImGui::TableNextColumn();
                ImGui::Text("100%%");

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right panel (fills remaining space)
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
        {
            if (ImGui::BeginTabBar("LightingTabs"))
            {
                if (ImGui::BeginTabItem("Properties"))
                {
                    ImGui::Text("Light Properties Content");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Groups"))
                {
                    ImGui::Text("Light Groups Content");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

} // namespace hdImgui
