/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "PresetEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowPresetEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Presets", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import Presets File");
        ImGui::Button("New Presets File");
        ImGui::Button("Save Presets File");
        ImGui::Button("Edit Presets");
        ImGui::Button("Load Presets File");
        ImGui::Button("Extract Presets from Current Config");      // overlay, context menus
    }  
    ImGui::End();
}
} // namespace hdImgui