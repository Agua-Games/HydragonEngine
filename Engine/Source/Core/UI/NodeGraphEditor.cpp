/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "NodeGraphEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowNodeGraphEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);

        if (ImGui::Begin("NodeGraph", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Open Graph");
            ImGui::Button("New Graph");
            ImGui::Button("Save Graph");
            ImGui::Button("Add Node");
            ImGui::Button("Group Selected Nodes"); 
            ImGui::Checkbox("Auto-Compile", &windowData->nodeGraphEditor_autoCompile);
            ImGui::Button("Compile Graph");
            ImGui::Button("Run Graph");
            ImGui::Button("Validate Graph");
        }
        ImGui::End();
    }
} // namespace hdImgui