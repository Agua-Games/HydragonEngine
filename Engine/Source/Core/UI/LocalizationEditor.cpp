/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "LocalizationEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowLocalizationEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Localization", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import Localization Config");
        ImGui::Button("New Localization Config");
        ImGui::Button("Load Localization Config");
        ImGui::Button("Save Localization Config");
        ImGui::Button("Export Localization Config");
        ImGui::Button("Share Localization Config");
        ImGui::Button("Localization Settings");         
    }          
    ImGui::End();
}
} // namespace hdImgui