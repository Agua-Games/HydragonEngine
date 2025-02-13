/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "MonetizationEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowMonetizationEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Monetization", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import Monetization Config");
            ImGui::Button("New Monetization Config");
            ImGui::Button("Save Monetization Config");
            ImGui::Button("Load Monetization Config");
            ImGui::Button("Export Monetization Config");
            ImGui::Button("Share Monetization Config");
            ImGui::Button("Monetization Settings");
            ImGui::Button("Connect Monetization Service");
            ImGui::Button("Connect to Sponsorship");
        } 
        ImGui::End();
    }
} // namespace hdImgui