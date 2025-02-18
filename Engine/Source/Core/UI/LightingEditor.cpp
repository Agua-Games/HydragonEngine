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
        ImGui::Text("Lights List");
        ImGui::EndChild();

        ImGui::SameLine();

        // Right panel (fills remaining space)
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
        ImGui::Text("Light Properties");
        ImGui::EndChild();
    }
    ImGui::End();
}

} // namespace hdImgui
