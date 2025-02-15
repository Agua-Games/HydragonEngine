/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "AgentsEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowAgentsEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Agents", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Add Agent");
        ImGui::Button("Load Model");
        ImGui::Button("Start Server");
        ImGui::Button("Stop Server");
        ImGui::Button("Start Client");
        ImGui::Button("Stop Client");
        ImGui::Button("Run Agent");
        ImGui::Button("Validate Agent");
        ImGui::Button("Ask Agent");
        ImGui::Button("Run Command");
        ImGui::Button("Validate Command");
        ImGui::Button("Add Command");
        ImGui::Button("Run Macro");
        ImGui::Button("Validate Macro");
        ImGui::Button("Add Macro");
        ImGui::Button("Load Macro");  
    }
    ImGui::End(); 
}
} // namespace hdImgui