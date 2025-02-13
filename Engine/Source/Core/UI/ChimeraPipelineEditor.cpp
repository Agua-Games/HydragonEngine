/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ChimeraPipelineEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowChimeraPipelineEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Chimera Pipeline", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Start Server");
            ImGui::Button("Refresh Server");
            ImGui::Button("Stop Server");
            ImGui::Button("Import Chimera Pipeline");
            ImGui::Button("New Chimera Pipeline");
            ImGui::Button("Save Chimera Pipeline");
            ImGui::Button("Load Livelink");
            ImGui::Button("Watch Livelink");
            ImGui::Button("Synchronize");
        } 
        ImGui::End();
    }
} // namespace hdImgui