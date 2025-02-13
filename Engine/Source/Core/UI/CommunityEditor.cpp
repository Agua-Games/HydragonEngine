/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "CommunityEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowCommunityEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Community", p_open, ImGuiWindowFlags_MenuBar))
        {
             ImGui::Button("Import Community Config");
            ImGui::Button("New Community Config");
            ImGui::Button("Save Community Config");
            ImGui::Button("Load Community Config");
            ImGui::Button("Export Community Config");
            ImGui::Button("Share Community Config");
            ImGui::Button("Report Issue");
            ImGui::Button("Report Bug");
            ImGui::Button("Ask Question");
            ImGui::Button("Community Settings");
            ImGui::Button("Request Feature");
            ImGui::Button("Request Support");
            ImGui::Button("Request Documentation");
            ImGui::Button("Request Training");
            ImGui::Button("Request Sponsorship");           // If the user has a Patreon account; have a project needing sponsorship
            ImGui::Button("Request Sponsorship Package");
            ImGui::Button("Request Sponsorship Support");
        }
        ImGui::End();
    }
} // namespace hdImgui