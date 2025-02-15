/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <string>

#include "ScriptEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowScriptEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Scripting", p_open, ImGuiWindowFlags_MenuBar))
    {
        // These will be moved to the menu bar later
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

        // Node Graph Integration
        if (ImGui::CollapsingHeader("Node Integration"))
        {
            ImGui::Button("Sync with Node");
            ImGui::Button("Preview Node Changes");
            ImGui::Button("Apply to Node");
            ImGui::Button("Revert Node Changes");
        }

        // AI Assistant Integration
        if (ImGui::CollapsingHeader("AI Assistant"))
        {
            ImGui::Button("Suggest Optimizations");
            ImGui::Button("Analyze Code");
            ImGui::Button("Generate Documentation");
            ImGui::Button("Explain Code");
        }

        // Code Editor Area
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        static char text[4096] = "";
        ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), 
                                 ImVec2(-1.0f, -1.0f), flags);  // Fixed: Using ImVec2 instead of ImVec::
    }
    ImGui::End();
}
} // namespace hdImgui
