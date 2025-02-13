/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "NetworkingEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowNetworkingEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Networking", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Start Server");
            ImGui::Button("Refresh Server");
            ImGui::Button("Stop Server");
            ImGui::Button("Load Networking Config");
            ImGui::Button("Share Networking Config");
            ImGui::Button("Networking Settings");
            ImGui::Button("Start Client");
            ImGui::Button("Refresh Client");
            ImGui::Button("Stop Client");
            ImGui::Button("Load Client Config");
            ImGui::Button("Share Client Config");
            ImGui::Button("Client Settings");
            ImGui::Button("Export Client Config");
        }       
        ImGui::End();
    }
} // namespace hdImgui