/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ProjectInsightsEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowProjectInsightsEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Project Insights", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Run Project Analysis");
            ImGui::Button("Stop Project Analysis");
            ImGui::Button("Send Analysis Results to Agent");
            ImGui::Button("Export Project Insights");
            ImGui::Button("Save Project Insights Config");
            ImGui::Button("Load Project Insights Config");
        }
        ImGui::End();
    }
} // namespace hdImgui