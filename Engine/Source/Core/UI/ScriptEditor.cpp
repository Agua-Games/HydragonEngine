/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ScriptEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowScriptEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Scripting", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import Script");
            ImGui::Button("New Script");
            ImGui::Button("Record Macro");
            ImGui::Button("Save Script");
            ImGui::Button("Compile Script");
            ImGui::Button("Run Script");
            ImGui::Button("Validate Script");
            ImGui::Button("Go To Line");
            ImGui::Button("Debug Script");
            ImGui::Button("Add Script");
            ImGui::Button("Ask Agent");
            ImGui::Button("Run Agent");
            ImGui::Button("Validate Agent");
        }
        ImGui::End();
    }
} // namespace hdImgui