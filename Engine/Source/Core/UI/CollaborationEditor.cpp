/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "CollaborationEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowCollaborationEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Collaboration", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Connect to Server");
            ImGui::Button("Disconnect from Server");
            ImGui::Button("Connect to Client Node");
            ImGui::Button("Disconnect from Client Node");
            ImGui::Button("Load Collaboration Config");
            ImGui::Button("Share Collaboration Config");
            ImGui::Button("Collaboration Settings");
        }   
        ImGui::End();
    }
} // namespace hdImgui