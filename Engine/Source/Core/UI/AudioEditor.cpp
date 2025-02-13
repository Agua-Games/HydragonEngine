/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "AudioEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowAudioEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Audio", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import Audio");
            ImGui::Button("New Audio");
            ImGui::Button("Save Audio");
            ImGui::Button("Load Audio");
            ImGui::Button("Play Audio");
            ImGui::Button("Stop Audio");
            ImGui::Button("Export Audio");
            ImGui::Button("Share Audio");
            ImGui::Button("Add Audio");
            ImGui::Button("Remove Audio");
        }
        ImGui::End();
    }
} // namespace hdImgui