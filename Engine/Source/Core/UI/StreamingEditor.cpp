/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "StreamingEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowStreamingEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Streaming", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Checkbox("Streaming Enabled", &windowData->streamingEnabled);
            ImGui::Button("Start Streaming Engine");
            ImGui::Button("Refresh Streaming Engine");
            ImGui::Button("Streaming Engine Settings");
        }
        ImGui::End();
    }
} // namespace hdImgui