/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "MontageEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowMontageEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Montage", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import Montage");
            ImGui::Button("New Montage");
            ImGui::Button("Save Montage");
            ImGui::Button("Load Montage");
            ImGui::Button("Play Montage");
            ImGui::Button("Stop Montage");
            ImGui::Button("Export Montage");
            ImGui::Button("Share Montage");
            ImGui::Button("Add Montage");
            ImGui::Button("Remove Montage");
        }
        ImGui::End();
    }
} // namespace hdImgui